#include "AST.h"

// ---- Number Expression ----
NumberExpr::NumberExpr(std::string val) : value(val) {}

void NumberExpr::print() const {
    std::cout << "NumberExpr(" << value << ")";
}

// ---- Variable Expression ----
VariableExpr::VariableExpr(std::string name) : name(name) {}

void VariableExpr::print() const {
    std::cout << "VariableExpr(" << name << ")";
}

// ---- Binary Expression ----
BinaryExpr::BinaryExpr(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

void BinaryExpr::print() const {
    std::cout << "BinaryExpr(";
    left->print();
    std::cout << " " << op.value << " ";
    right->print();
    std::cout << ")";
}

// ---- Let Statement ----
LetStatement::LetStatement(std::string name, std::unique_ptr<Expression> value)
    : name(name), value(std::move(value)) {}

void LetStatement::print() const {
    std::cout << "LetStatement(" << name << " = ";
    value->print();
    std::cout << ")";
}

// ---- Var Statement ----
VarStatement::VarStatement(std::string name, std::unique_ptr<Expression> value)
    : name(name), value(std::move(value)) {}

void VarStatement::print() const {
    std::cout << "VarStatement(" << name << " = ";
    value->print();
    std::cout << ")";
}

// ---- Assign Statement ----
AssignStatement::AssignStatement(std::string name, std::unique_ptr<Expression> value)
    : name(name), value(std::move(value)) {}

void AssignStatement::print() const {
    std::cout << "AssignStatement(" << name << " = ";
    value->print();
    std::cout << ")";
}

// ---- Return Statement ----
ReturnStatement::ReturnStatement(std::unique_ptr<Expression> value)
    : value(std::move(value)) {}

void ReturnStatement::print() const {
    std::cout << "ReturnStatement(";
    value->print();
    std::cout << ")";
}

// ---- Function Declaration ----
FunctionDecl::FunctionDecl(std::string name, std::vector<std::string> params, std::vector<std::unique_ptr<Statement>> body)
    : name(name), params(std::move(params)), body(std::move(body)) {}

void FunctionDecl::print() const {
    std::cout << "FunctionDecl(" << name << " (";
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << params[i];
        if (i < params.size() - 1) std::cout << ", ";
    }
    std::cout << ") { ";
    for (const auto& stmt : body) {
        stmt->print();
        std::cout << "; ";
    }
    std::cout << "})";
}
