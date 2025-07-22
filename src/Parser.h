#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <unordered_map>

static std::unordered_map<TokenType , int> PRECEDENCE = {
    {TokenType::STAR , 5} , {TokenType::SLASH , 5} , {TokenType::MODULO , 5},
    {TokenType::PLUS , 4} , {TokenType::MINUS , 4} ,
    {TokenType::LESS , 3} , {TokenType::LESS_EQUAL , 3},
    {TokenType::GREATER , 3} , {TokenType::GREATER_EQUAL , 3},
    {TokenType::EQUAL , 2} , {TokenType::NOT_EQUAL , 2},
    {TokenType::AND , 1} , {TokenType::OR , 1} , {TokenType::NOT , 1} , {TokenType::XOR , 1},
};

class Parser {
    private:
        std::vector<Token> tokens;
        size_t index;

        // Helpers
        Token peek() const;
        Token peek(size_t offset) const;
        Token consume();
        bool match(TokenType type);
        void expect(TokenType type , const std::string& errorMessage);

        // Parsing primitives
        std::unique_ptr<Expression> parseExpression();
        std::unique_ptr<Expression> parsePrimary();
        std::unique_ptr<Expression> parseBinary(int minPrecedence);
        std::unique_ptr<Expression> parseCallOrPrimary();
        std::vector<std::unique_ptr<Statement>> parseBlock();

        // Statements
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Statement> parseLetStatement();
        std::unique_ptr<Statement> parseVarStatement();
        std::unique_ptr<Statement> parseAssignStatement();
        std::unique_ptr<Statement> parseIfStatement();
        std::unique_ptr<Statement> parseWhileStatement();
        std::unique_ptr<Statement> parseForStatement();
        std::unique_ptr<Statement> parseMatchStatement();
        std::unique_ptr<Statement> parseReturnStatement();
        
        // Functions
        std::unique_ptr<FunctionDecl> parseFunction();
    
    public:
        Parser(std::vector<Token> tokens);
        std::vector<std::unique_ptr<FunctionDecl>> parseProgram();     
};


#endif //PARSER_H