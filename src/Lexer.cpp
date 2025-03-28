#include "Lexer.h"
#include <cctype>
#include <unordered_map>
#include <iostream>

std::unordered_map<std::string, TokenType> keywords = {
    {"fn", TokenType::FN},
    {"class", TokenType::CLASS},
    {"let", TokenType::LET},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"var",TokenType::VAR}
};

Lexer::Lexer(const std::string& source) : source(source) , index(0) , line(1) {
    currentChar = source.empty() ? '\0' : source[0];
}

void Lexer::advance() {
    index++;
    if(index >= source.size()) {
        currentChar = '\0'; //END OF FILE
    } else {
        currentChar = source[index];
    }
}

void Lexer::skipWhitespace() {
    while(isspace(currentChar)) {
        if(currentChar == '\n') line++;
        advance();
    }
}

Token Lexer::number() {
    std::string num;
    bool isFloat = false;

    while(isdigit(currentChar) || currentChar == '.') {
        if(currentChar == '.') {
            if(isFloat) break; //SECOND DOT IS NOT ALLOWED
            isFloat = true;
        }
        num += currentChar;
        advance();
    }
    return isFloat ? Token(TokenType::FLOAT_LITERAL,num,line) : Token(TokenType::INTEGER_LITERAL,num,line);
}

Token Lexer::identifier() {
    std::string id;
    while(isalnum(currentChar) || currentChar == '_') {
        id += currentChar;
        advance();
    }
    if(keywords.find(id) != keywords.end()) {
        return Token(keywords[id],id,line);
    }
    return Token(TokenType::IDENTIFIER,id,line);
}

Token Lexer::stringLiteral() {
    advance(); //SKIP THE OPENING QUOTE
    std::string str;
    while(currentChar != '"' && currentChar != '\0') {
        str += currentChar;
        advance();
    }
    advance(); //SKIP THE CLOSING QUOTE
    return Token(TokenType::STRING_LITERAL,str,line);
}

Token Lexer::nextToken() {
    skipWhitespace();
    if(isdigit(currentChar)) return number();
    if(isalpha(currentChar) || currentChar == '_') return identifier();
    if(currentChar == '"') return stringLiteral();

    switch (currentChar) {
        case '+' : advance(); return Token(TokenType::PLUS,"+",line);
        case '-' : advance(); return Token(TokenType::MINUS,"-",line);
        case '*' : advance(); return Token(TokenType::STAR,"*",line);
        case '/' : advance(); return Token(TokenType::SLASH,"/",line);
        case '%' : advance(); return Token(TokenType::MODULO,"%",line);
        case '=' :
            advance();
            if(currentChar == '=') { advance(); return Token(TokenType::EQUAL,"==",line); }
            return Token(TokenType::ASSIGN,"=",line);
        case '!' :
            advance();
            if(currentChar == '=') { advance(); return Token(TokenType::NOT_EQUAL,"!=",line); }  
            return Token(TokenType::IDENTIFIER,"!",line);
        case '<' :
            advance();
            if(currentChar == '=') { advance(); return Token(TokenType::LESS_EQUAL,"<=",line); }
            return Token(TokenType::LESS,"<",line);   
        case '>' :
            advance();
            if(currentChar == '=') { advance(); return Token(TokenType::GREATER_EQUAL,">=",line); }
            return Token(TokenType::GREATER,">",line);
        case '(' : advance(); return Token(TokenType::LPAREN,"(",line);
        case ')' : advance(); return Token(TokenType::RPAREN,")",line);
        case '{' : advance(); return Token(TokenType::LBRACE,"{",line);
        case '}' : advance(); return Token(TokenType::RBRACE,"}",line);
        case '[' : advance(); return Token(TokenType::LBRACKET,"[",line);
        case ']' : advance(); return Token(TokenType::RBRACKET,"]",line);
        case ',' : advance(); return Token(TokenType::COMMA,",",line);
        case ';' : advance(); return Token(TokenType::SEMICOLON,";",line);
        case '\0' : advance(); return Token(TokenType::END_OF_FILE,"EOF",line);
        default :
            std::cerr<<"Unexpected character : "<<currentChar<<" at line "<<line<<"\n";
            advance();
            return Token(TokenType::END_OF_FILE,"EOF",line);           
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while(currentChar != '\0') {
        tokens.push_back(nextToken());
    }
    tokens.push_back(Token(TokenType::END_OF_FILE,"EOF",line));
    return tokens;
}