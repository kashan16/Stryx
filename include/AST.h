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

class IfStatement : public Statement {
    public :
        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> thenBranch;
        std::vector<std::unique_ptr<Statement>> elseBranch;

        IfStatement(
            std::unique_ptr<Expression> cond,
            std::vector<std::unique_ptr<Statement>> thenB,
            std::vector<std::unique_ptr<Statement>> elseB
        ) : condition(std::move(cond)) 
          , thenBranch(std::move(thenB))
          , elseBranch(std::move(elseB))
          {}
        void print() const override;  
};

class WhileStatement : public Statement {
    public :
        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> body;

        WhileStatement(
            std::unique_ptr<Expression> cond,
            std::vector<std::unique_ptr<Statement>> body
        ) : condition(std::move(cond)) , body(std::move(body)) {}
        
        void print() const override;
};

class ForStatement : public Statement {
    public :
        std::string iteratorName;
        std::unique_ptr<Expression> iterable;
        std::vector<std::unique_ptr<Statement>> body;

        ForStatement (
            std::string itName,
            std::unique_ptr<Expression> iterable,
            std::vector<std::unique_ptr<Statement>> body 
        ) : iteratorName(std::move(itName))
          , iterable(std::move(iterable))
          , body(std::move(body))
        {}
        void print() const override;
};

class CallExpr : public Expression {
    public :
        std::unique_ptr<Expression> callee;
        std::vector<std::unique_ptr<Expression>> arguments;

        CallExpr(
            std::unique_ptr<Expression> callee,
            std::vector<std::unique_ptr<Expression>> args
        ) : callee(std::move(callee)) , arguments(std::move(args)) {}

        void print() const override;
};

struct MatchArm
{
    std::unique_ptr<Expression> pattern;
    std::vector<std::unique_ptr<Statement>> body;
    MatchArm(std::unique_ptr<Expression> pat , std::vector<std::unique_ptr<Statement>> stmts)
        : pattern(std::move(pat)) , body(std::move(stmts)) {}
};

class MatchStatement : public Statement {
    public : 
        std::unique_ptr<Expression> expr;
        std::vector<MatchArm> arms;

        MatchStatement(std::unique_ptr<Expression> expr , std::vector<MatchArm> arms)
            : expr(std::move(expr)) , arms(std::move(arms)) {}

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
