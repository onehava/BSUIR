#include "Expression.h"

Expression::Expression(Type type){
    m_type = type;
}

Expression::Type Expression::type() const {
    return m_type;
}

void Expressions::push_back(Expression_ptr expression){
    m_container.emplace_back(std::move(expression));
}

Expressions::iterator Expressions::begin() const {
    auto ptr = const_cast<container_t::pointer>(m_container.data());
    return {ptr};
}

Expressions::iterator Expressions::end() const {
    auto ptr = const_cast<container_t::pointer>(m_container.data() +
                                                m_container.size());
    return {ptr};
}

Expressions::iterator::iterator(container_t::pointer pointer){
    m_pointer = pointer;
}

const Expression &Expressions::iterator::operator*() const {
    return **m_pointer;
}

Expressions::iterator &Expressions::iterator::operator++(){
    ++m_pointer;
    return *this;
}

bool Expressions::iterator::operator==(const iterator &other) const {
    return m_pointer == other.m_pointer;
}

bool Expressions::iterator::operator!=(const iterator &other) const {
    return m_pointer != other.m_pointer;
}

Unary::Unary(Lexeme::Type op, Expression_ptr expr)
        : Expression(Type::Unary){
    m_op = op;
    m_expr = std::move(expr);
}

Lexeme::Type Unary::operation() const {
    return m_op;
}

const Expression &Unary::expression() const {
    return *m_expr;
}

Binary::Binary(Lexeme::Type op, Expression_ptr left, Expression_ptr right)
        : Expression(Type::Binary){
    m_op = op;
    m_left = std::move(left);
    m_right = std::move(right);
}

Lexeme::Type Binary::operation() const {
    return m_op;
}

const Expression &Binary::left() const {
    return *m_left;
}

const Expression &Binary::right() const {
    return *m_right;
}

Numeric::Numeric(double value)
        : Expression(Type::Numeric){
    m_value = value;
}

double Numeric::value() const {
    return m_value;
}

String::String(std::string&& value)
        : Expression(Type::String){
    m_value = std::move(value);
}

const std::string &String::value() const {
    return m_value;
}

Ident::Ident(std::string &&name)
        : Expression(Type::Ident){
    m_name = std::move(name);
}

const std::string &Ident::name() const {
    return m_name;
}