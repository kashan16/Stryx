#include "Parser.h"
#include "AST.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) , index(0) {}

Token Parser::peek() const {
    return (index < tokens.size()) ? tokens[index] : Token(TokenType::END_OF_FILE ,"EOF",-1);
}

Token Parser::peek(size_t offsett) const {
    size_t pos = index + offsett;
    return (pos < tokens.size() ? tokens[pos] : Token(TokenType::END_OF_FILE , "EOF",-1));
}

Token Parser::consume() {
    return (index < tokens.size() ? tokens[index++] : Token(TokenType::END_OF_FILE , "EOF" , -1));
}

bool Parser::match(TokenType type) {
    if(peek().type == type) {
        consume();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const std::string& errMsg) {
    if (!match(type)) {
        auto tk = peek();
        std::cerr << "Parse Error: " << errMsg
                  << " at line " << tk.line
                  << ", got '" << tk.value << "'\n";
        exit(1);
    }
}

std::vector<std::unique_ptr<FunctionDecl>> Parser::parseProgram() {
    std::vector<std::unique_ptr<FunctionDecl>> functions;
    while(peek().type != TokenType::END_OF_FILE) {
        functions.push_back(parseFunction());
    }
    return functions;
}

std::unique_ptr<FunctionDecl> Parser::parseFunction() {
    expect(TokenType::FN , "expected 'fn' to start function");
    Token name = consume();
    expect(TokenType::LPAREN , "expected '(' after function name");

    std::vector<std::string> params;
    if(peek().type != TokenType::RPAREN) {
        do {
            Token p = consume();
            if(p.type != TokenType::IDENTIFIER) {
                std::cerr << "Parse Error : expected parameter name , got '" << p.value <<"'\n";
                exit(1);
            }
            params.push_back(p.value);
        } while(match(TokenType::COMMA));
    }
    expect(TokenType::RPAREN,"expected ')' after parameters");
    expect(TokenType::LBRACE,"expected '{' before function body");

    std::vector<std::unique_ptr<Statement>> body;
    while(!match(TokenType::RBRACE)) {
        body.push_back(parseStatement());
    }

    return std::make_unique<FunctionDecl>(name.value , std::move(params) , std::move(body)); 
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if(match(TokenType::LET)) return parseLetStatement();
    if(match(TokenType::VAR)) return parseVarStatement();
    if(match(TokenType::RETURN)) return parseReturnStatement();
    if (match(TokenType::IF))    return parseIfStatement();
    if (match(TokenType::WHILE)) return parseWhileStatement();  
    if (match(TokenType::FOR))   return parseForStatement();
    if (match(TokenType::MATCH)) return parseMatchStatement();
    if(peek().type == TokenType::IDENTIFIER && peek(1).type == TokenType::ASSIGN) return parseAssignStatement();

    std::cerr << "Parse Error : unexpected statement at line "<< peek().line <<"\n";
    exit(1);
}

std::unique_ptr<Statement> Parser::parseLetStatement() {
    Token name = consume();
    expect(TokenType::ASSIGN , "expected '=' after variable name");
    auto value = parseExpression();
    expect(TokenType::SEMICOLON , "expected ';' after let declaration");
    return std::make_unique<LetStatement>(name.value , std::move(value));
}

std::unique_ptr<Statement> Parser::parseVarStatement() {
    Token name = consume();
    expect(TokenType::ASSIGN , "expected '=' after variable name");
    auto value = parseExpression();
    expect(TokenType::SEMICOLON , "expected ';' after var declaration");
    return std::make_unique<VarStatement>(name.value , std::move(value));
}

std::unique_ptr<Statement> Parser::parseAssignStatement() {
    Token name = consume();                      
    expect(TokenType::ASSIGN, "expected '=' in assignment");
    auto value = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after assignment");
    return std::make_unique<AssignStatement>(name.value, std::move(value));
}

std::unique_ptr<Statement> Parser::parseReturnStatement() {
    auto value = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after return");
    return std::make_unique<ReturnStatement>(std::move(value));
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    expect(TokenType::LPAREN , "expected '(' after if");
    auto cond = parseExpression();
    expect(TokenType::RPAREN , "expected ')' after condition");
    expect(TokenType::LBRACE , "expected '{' then");
    std::vector<std::unique_ptr<Statement>> thenStmts = parseBlock() ;
    std::vector<std::unique_ptr<Statement>> elseStmts;
    
    if(match(TokenType::ELSE)) {
        if(match(TokenType::IF)) {
            elseStmts.push_back(parseIfStatement());
        } else {
            expect(TokenType::LBRACE , "expected '{' after else");
            elseStmts = parseBlock(); 
        }
    }
    return std::make_unique<IfStatement>(std::move(cond) , std::move(thenStmts) , std::move(elseStmts));
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    expect(TokenType::LPAREN , "expected '(' after while");
    auto cond = parseExpression();
    expect(TokenType::RPAREN , "expected ')' after condition");
    expect(TokenType::LBRACE , "expected '{' after while");
    auto body = parseBlock();
    return std::make_unique<WhileStatement>(std::move(cond) , std::move(body));
}

std::unique_ptr<Statement> Parser::parseForStatement() {
    Token itname = consume();
    expect(TokenType::IDENTIFIER , "expected iterator name");
    expect(TokenType::IN , "expected 'in' in for");
    auto iterable = parseExpression();
    expect(TokenType::LBRACE , "expected '{' after for");
    auto body = parseBlock();
    return std::make_unique<ForStatement>(itname.value , std::move(iterable) , std::move(body));
}

std::unique_ptr<Statement> Parser::parseMatchStatement() {
    auto expr = parseExpression();

    expect(TokenType::LBRACE, "expected '{' after match expression");

    std::vector<MatchArm> arms;
    while (peek().type != TokenType::RBRACE && peek().type != TokenType::END_OF_FILE) {
        std::unique_ptr<Expression> pat;
        Token t = peek();
        if (t.type == TokenType::INTEGER_LITERAL || t.type == TokenType::FLOAT_LITERAL ||
            t.type == TokenType::STRING_LITERAL || t.type == TokenType::IDENTIFIER) {
            pat = parsePrimary();
        } else if (t.type == TokenType::UNDERSCORE) {
            consume();
            pat = std::make_unique<VariableExpr>("_");
        } else {
            std::cerr << "Parse Error: unexpected pattern '" 
                      << t.value << "' in match arm at line " 
                      << t.line << "\n";
            exit(1);
        }

        expect(TokenType::ARROW, "expected '=>' after match pattern");
        std::vector<std::unique_ptr<Statement>> body;
        if (match(TokenType::LBRACE)) {
            while (!match(TokenType::RBRACE)) {
                body.push_back(parseStatement());
            }
        } else {
            body.push_back(parseStatement());
        }

        arms.emplace_back(std::move(pat), std::move(body));
        match(TokenType::COMMA);
    }

    expect(TokenType::RBRACE, "expected '}' to close match");
    
    return std::make_unique<MatchStatement>(std::move(expr), std::move(arms));
}


std::vector<std::unique_ptr<Statement>> Parser::parseBlock() {
    std::vector<std::unique_ptr<Statement>> stmts;
    while(!match(TokenType::RBRACE) && peek().type != TokenType::END_OF_FILE) {
        stmts.push_back(parseStatement());
    }
    return stmts;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseBinary(0);
}

std::unique_ptr<Expression> Parser::parseBinary(int minPerc) {
    auto left = parsePrimary();

    while(true) {
        auto op = peek();
        int prec = PRECEDENCE[op.type];
        if(prec < minPerc) break;
        consume();
        auto right = parseBinary(prec+1);
        left = std::make_unique<BinaryExpr>(std::move(left) , op , std::move(right));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    Token tok = consume();
    if(tok.type == TokenType::INTEGER_LITERAL || tok.type == TokenType::FLOAT_LITERAL) {
        return std::make_unique<NumberExpr>(tok.value);
    }
    if(tok.type == TokenType::IDENTIFIER) {
        return std::make_unique<VariableExpr>(tok.value);
    } 
    std::cerr << "Parse Error : unexpected token '" << tok.value << "' at line" << tok.line << "\n";
    exit(1);
}

std::unique_ptr<Expression> Parser::parseCallOrPrimary() {
    auto expr = parsePrimary();
    while(match(TokenType::LPAREN)) {
        std::vector<std::unique_ptr<Expression>> args;
        if(peek().type != TokenType::RPAREN) {
            do {
                args.push_back(parseExpression());
            } while(match(TokenType::COMMA));
        }
        expect(TokenType::RPAREN , "expected ')' after call args");
        expr = std::make_unique<CallExpr>(std::move(expr) , std::move(args)); 
    }
    return expr; 
}


