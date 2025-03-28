#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
    private :  
        std::string source;
        int index;
        int line; 
        char currentChar;

        void advance();
        void skipWhitespace();
        Token number();
        Token identifier();
        Token stringLiteral();
        Token nextToken();
    
    public :
        Lexer(const std::string& source);
        std::vector<Token> tokenize();  
};

#endif