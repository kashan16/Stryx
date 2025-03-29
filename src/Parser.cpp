#include "Parser.h"
#include "AST.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) , index(0) {}

Token Parser::peek() {
    return (index < tokens.size()) ? tokens[index] : Token(TokenType::END_OF_FILE ,"EOF",-1);
}

Token Parser::consume() {
    return tokens[index++];
}

bool Parser::match(TokenType type) {
    if(peek().type == type) {
        consume();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type , const std::string errorMessage) {
    if(!match(type)) {
        std::cerr<<"Parse Error : "<<errorMessage<<" at line "<<peek().line<<std::endl;
        exit(1);
    }
}

//Expression Parsing
std::unique_ptr<Expression> Parser::parseExpression() {
    return parseBinary(0);
} 

std::unique_ptr<Expression> Parser::parsePrimary() {
    Token token = consume();

    if(token.type == TokenType::IDENTIFIER) {
        return std::make_unique<VariableExpr>(token.value);
    } else if(token.type == TokenType::INTEGER_LITERAL || token.type == TokenType::FLOAT_LITERAL) {
        return std::make_unique<NumberExpr>(token.value);
    }

    std::cerr<<"Parse Error : Unexpected token '"<<token.value<<"' at line "<<token.line<<std::endl;
    exit(1);
}

std::unique_ptr<Expression> Parser::parseBinary(int precedence) {
    std::unique_ptr<Expression> left = parsePrimary();
    while(true) {
        Token op = peek();
        if(op.type != TokenType::PLUS && op.type != TokenType::MINUS && op.type != TokenType::STAR && op.type != TokenType::SLASH && op.type != TokenType::MODULO) break;
        consume();
        std::unique_ptr<Expression> right = parsePrimary();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}

//Statement Parsing
std::unique_ptr<Statement> Parser::parseStatement() {
    if(match(TokenType::LET)) {
        return parseLetStatement();
    } else if(match(TokenType::VAR)) {
        return parseVarStatement();
    } else if(match(TokenType::ASSIGN)) {
        return parseAssignStatement();
    } else if(match(TokenType::RETURN)) {
        return parseReturnStatement();
    }
    std::cerr<<"Parse Error : Unexpected statement at line "<<peek().line<<std::endl;
    exit(1);
}

std::unique_ptr<Statement> Parser::parseLetStatement() {
    Token name = consume();
    expect(TokenType::ASSIGN,"Expected '=' after variable name");
    std::unique_ptr<Expression> value = parseExpression();
    expect(TokenType::SEMICOLON,"Expected ';' after variable declaration");
    return std::make_unique<LetStatement>(name.value,std::move(value));
}

std::unique_ptr<Statement> Parser::parseVarStatement() {
    Token name = consume();
    expect(TokenType::ASSIGN,"Expected '=' after variable name");
    std::unique_ptr<Expression> value = parseExpression();
    expect(TokenType::SEMICOLON,"Expected ';' after variable declaration");
    return std::make_unique<VarStatement>(name.value,std::move(value));
}

std::unique_ptr<Statement> Parser::parseAssignStatement() {
    Token name = consume();
    expect(TokenType::ASSIGN,"Expected '=' for assignment");
    std::unique_ptr<Expression> value = parseExpression();
    expect(TokenType::SEMICOLON,"Expected ';' after assignment");
    return std::make_unique<AssignStatement>(name.value,std::move(value));
}

std::unique_ptr<Statement> Parser::parseReturnStatement() {
    std::unique_ptr<Expression> value = parseExpression();
    expect(TokenType::SEMICOLON,"Expected ';' after return statement");
    return std::make_unique<ReturnStatement>(std::move(value));
}

//Function Parsing
std::unique_ptr<FunctionDecl> Parser::parseFunction() {
    expect(TokenType::FN,"Expected 'fn' at start of function");
    Token name = consume();
    expect(TokenType::LPAREN,"Expected '(' after function name");

    std::vector<std::string> params;
    while(peek().type != TokenType::RPAREN) {
        params.push_back(consume().value);
        if(!match(TokenType::COMMA)) break;
    }
    expect(TokenType::RPAREN,"Expected ')' after parameters");
    expect(TokenType::LBRACE,"Expected '{' before function body");
    std::vector<std::unique_ptr<Statement>> body;
    while(!match(TokenType::RBRACE)) {
        body.push_back(parseStatement());
    }
    return std::make_unique<FunctionDecl>(name.value,params,std::move(body));
}

std::unique_ptr<FunctionDecl> Parser::parse() {
    return parseFunction();
}