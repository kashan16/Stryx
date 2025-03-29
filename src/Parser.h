#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "AST.h"
#include <memory>

class Parser {
    private:
        std::vector<Token> tokens;
        int index;

        Token peek();
        Token consume();
        bool match(TokenType type);
        void expect(TokenType type , const std::string errorMessage);

        std::unique_ptr<Expression> parseExpression();
        std::unique_ptr<Expression> parsePrimary();
        std::unique_ptr<Expression> parseBinary(int precedence);

        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Statement> parseLetStatement();
        std::unique_ptr<Statement> parseVarStatement();
        std::unique_ptr<Statement> parseAssignStatement();
        std::unique_ptr<Statement> parseReturnStatement();
        
        std::unique_ptr<FunctionDecl> parseFunction();
    
    public:
        Parser(std::vector<Token> tokens);
        std::unique_ptr<FunctionDecl> parse();     
};


#endif //PARSER_H