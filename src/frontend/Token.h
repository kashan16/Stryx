#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Enum for token types
enum class TokenType {
    // Keywords
    FN, CLASS, LET , VAR , RETURN, IF, ELSE, FOR, WHILE, BREAK, CONTINUE,
    
    // Data Types
    INT, FLOAT, STRING, BOOL, VOID,

    // Operators
    PLUS, MINUS, STAR, SLASH, MODULO, 
    ASSIGN, EQUAL, NOT_EQUAL, 
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,

    // Separators
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, COMMA, SEMICOLON,

    // Identifiers & Literals
    IDENTIFIER, INTEGER_LITERAL, FLOAT_LITERAL, STRING_LITERAL,

    // End of File
    END_OF_FILE
};

// Token class
class Token {
public:
    TokenType type;
    std::string value;
    int line;

    Token(TokenType type, std::string value, int line);
    std::string toString() const;
};

#endif // TOKEN_H
