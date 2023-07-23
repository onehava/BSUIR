#pragma once

#include "Lexeme.h"
#include <memory>
#include <vector>

class Expression {
public:
    enum class Type : uint8_t {
        Unary,
        Binary,
        Numeric, String, Ident,
    };

    Expression(Type type);
    Type type() const;

private:
    Type m_type;
};

using Expression_ptr = std::unique_ptr<Expression>;

class Expressions {
public:
    class iterator;
    void push_back(Expression_ptr expression);

    iterator begin() const;
    iterator end() const;

private:
    using container_t = std::vector<Expression_ptr>;
    container_t m_container;
};

class Expressions::iterator {
public:
    iterator() = default;

    const Expression& operator*() const;
    iterator& operator++();

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

private:
    friend class Expressions;
    iterator(container_t::pointer pointer);

    container_t ::pointer m_pointer;
};

class Unary : public Expression {
public:
    Unary(Lexeme::Type op, Expression_ptr expr);
    Lexeme::Type operation() const;
    const Expression& expression() const;

private:
    Lexeme::Type m_op;
    Expression_ptr m_expr;
};

class Binary : public Expression {
public:
    Binary(Lexeme::Type op, Expression_ptr left, Expression_ptr right);
    Lexeme::Type operation() const;
    const Expression& left() const;
    const Expression& right() const;

private:
    Lexeme::Type m_op;
    Expression_ptr m_left, m_right;
};

class Numeric : public Expression {
public:
    Numeric(double value);
    double value() const;

private:
    double m_value;
};

class String : public Expression {
public:
    String(std::string&& value);
    const std::string& value() const;

private:
    std::string m_value;
};

class Ident : public Expression {
public:
    Ident(std::string&& name);
    const std::string& name() const;

private:
    std::string m_name;
};