#include "Token.h"
#include <iostream>

Token::Token(TokenType type , std::string value , int line) :
    type(type) , value(value) , line(line) {}

std::string Token::toString() const {
    return "Token (" + value + ", line " + std::to_string(line) + ")";
}    