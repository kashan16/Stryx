#ifndef AST_H
#define AST_H

#include "Token.h"
#include <iostream>
#include <vector>
#include <memory>

// ---- Base class for all AST nodes ----
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print() const = 0;  // Pure virtual function for debugging
};

// ---- EXPRESSION NODES ----
class Expression : public ASTNode {}; // Base class for expressions

class NumberExpr : public Expression {
public:
    std::string value;
    NumberExpr(std::string val);
    void print() const override;  // Declare print() properly
};

class VariableExpr : public Expression {
public:
    std::string name;
    VariableExpr(std::string name);
    void print() const override;
};

class BinaryExpr : public Expression {
public:
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;

    BinaryExpr(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right);
    void print() const override;
};

// ---- STATEMENT NODES ----
class Statement : public ASTNode {};

class LetStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    
    LetStatement(std::string name, std::unique_ptr<Expression> value);
    void print() const override;
};

class VarStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    
    VarStatement(std::string name, std::unique_ptr<Expression> value);
    void print() const override;
};

class AssignStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;

    AssignStatement(std::string name, std::unique_ptr<Expression> value);
    void print() const override;
};

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value;
    
    ReturnStatement(std::unique_ptr<Expression> value);
    void print() const override;
};

class FunctionDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<Statement>> body;
    
    FunctionDecl(std::string name, std::vector<std::string> params, std::vector<std::unique_ptr<Statement>> body);
    void print() const override;
};

#endif // AST_H
