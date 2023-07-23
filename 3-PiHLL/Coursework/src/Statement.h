#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "Expression.h"

class Statement {
public:
    enum class Type : uint8_t {
        Assign,
        If, For, While,
        Input, Print,
    };

    Statement(Type type);
    Type type() const;

private:
    Type m_type;
};

using Statement_ptr = std::unique_ptr<Statement>;

class Statements {
public:
    class iterator;

    void push_back(Statement_ptr statement);

    iterator begin() const;
    iterator end() const;

private:
    using container_t = std::vector<Statement_ptr>;
    container_t m_container;
};

class Statements::iterator {
public:
    iterator() = default;

    const Statement& operator*() const;
    iterator& operator++();

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

private:
    friend class Statements;
    iterator(container_t::pointer pointer);

    container_t::pointer m_pointer;
};

class Assign : public Statement {
public:
    Assign(std::string&& ident, std::unique_ptr<Expression> expr);
    const std::string& ident() const;
    const Expression& expression() const;

private:
    std::string m_ident;
    std::unique_ptr<Expression> m_expr;
};

class If : public Statement {
public:
    If(std::unique_ptr<Expression> condition, Statements if_branch);
    If(std::unique_ptr<Expression> condition, Statements if_branch,
       Statements else_branch);
    const Expression& condition() const;
    const Statements& if_branch() const;
    const Statements& else_branch() const;

private:
    std::unique_ptr<Expression> m_condition;
    Statements m_if_branch, m_else_branch;
};

class For : public Statement {
public:
    For(std::string&& ident, Expression_ptr start, Expression_ptr end,
        Statements statements);
    const std::string& ident() const;
    const Expression& start() const;
    const Expression& end() const;
    const Statements& body() const;

private:
    std::string m_ident;
    Expression_ptr m_start, m_end;
    Statements m_body;
};

class While : public Statement {
public:
    While(std::unique_ptr<Expression> condition, Statements body);
    const Expression& condition() const;
    const Statements& body() const;

private:
    std::unique_ptr<Expression> m_condition;
    Statements m_body;
};

class Input : public Statement {
public:
    Input(std::string&& ident);
    Input(std::string&& message, std::string&& ident);
    const std::string& ident() const;
    const std::string& message() const;

private:
    std::string m_ident, m_message;
};

class Print : public Statement {
public:
    Print(Expressions params);
    const Expressions& params() const;

private:
    Expressions m_params;
};