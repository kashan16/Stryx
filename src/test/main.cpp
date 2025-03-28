#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Token.h"

void runLexer(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    for(const Token& token : tokens) {
        std::cout<<token.toString()<<std::endl;
    }
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if(!file) {
        std::cerr<<"Error : Could not open file "<<filename<<std::endl;
        exit(1);
    }
    std::string content((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    file.close();
    return content;
}

int main(int argc , char* argv[]) {
    if(argc < 2) {
        std::cerr<<"Usage : ./stryx_lexer <filename.styx>"<<std::endl;
        return 1;
    }
    std::string source = readFile(argv[1]);
    runLexer(source);

    return 0;
}