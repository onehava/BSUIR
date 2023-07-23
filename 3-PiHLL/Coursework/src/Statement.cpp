#include "Statement.h"

Statement::Statement(Type type){
    m_type = type;
}

Statement::Type Statement::type() const {
    return m_type;
}

void Statements::push_back(Statement_ptr statement){
    m_container.emplace_back(std::move(statement));
}

Statements::iterator Statements::begin() const {
    auto ptr = const_cast<container_t::pointer>(m_container.data());
    return {ptr};
}

Statements::iterator Statements::end() const {
    auto ptr = const_cast<container_t::pointer>(m_container.data() +
                                                m_container.size());
    return {ptr};
}

Statements::iterator::iterator(container_t::pointer pointer){
    m_pointer = pointer;
}

const Statement &Statements::iterator::operator*() const {
    return **m_pointer;
}

Statements::iterator &Statements::iterator::operator++(){
    ++m_pointer;
    return *this;
}

bool Statements::iterator::operator==(const iterator &other) const {
    return m_pointer == other.m_pointer;
}

bool Statements::iterator::operator!=(const iterator &other) const {
    return m_pointer != other.m_pointer;
}

Assign::Assign(std::string&& ident, std::unique_ptr<Expression> expr)
        : Statement(Type::Assign){
    m_ident = std::move(ident);
    m_expr = std::move(expr);
}

const std::string& Assign::ident() const {
    return m_ident;
}

const Expression &Assign::expression() const {
    return *m_expr;
}

If::If(std::unique_ptr<Expression> condition, Statements if_branch)
        : Statement(Type::If){
    m_condition = std::move(condition);
    m_if_branch = std::move(if_branch);
}

If::If(std::unique_ptr<Expression> condition, Statements if_branch,
       Statements else_branch)
        : Statement(Type::If){
    m_condition = std::move(condition);
    m_if_branch = std::move(if_branch);
    m_else_branch = std::move(else_branch);
}

const Expression &If::condition() const {
    return *m_condition;
}

const Statements &If::if_branch() const {
    return m_if_branch;
}

const Statements &If::else_branch() const {
    return m_else_branch;
}

For::For(std::string&& ident, Expression_ptr start, Expression_ptr end,
         Statements body)
        : Statement(Type::For){
    m_ident = std::move(ident);
    m_start = std::move(start);
    m_end = std::move(end);
    m_body = std::move(body);
}

const std::string &For::ident() const {
    return m_ident;
}

const Expression &For::start() const {
    return *m_start;
}

const Expression &For::end() const {
    return *m_end;
}

const Statements &For::body() const {
    return m_body;
}

While::While(std::unique_ptr<Expression> condition, Statements body)
        : Statement(Type::While){
    m_condition = std::move(condition);
    m_body = std::move(body);
}

const Expression &While::condition() const {
    return *m_condition;
}

const Statements &While::body() const {
    return m_body;
}

Input::Input(std::string&& ident) : Statement(Type::Input){
    m_ident = std::move(ident);
}

Input::Input(std::string&& message, std::string&& ident)
        : Statement(Type::Input){
    m_ident = std::move(ident);
    m_message = std::move(message);
}

const std::string& Input::ident() const {
    return m_ident;
}

const std::string& Input::message() const {
    return m_message;
}

Print::Print(Expressions params) : Statement(Type::Print){
    m_params = std::move(params);
}

const Expressions &Print::params() const {
    return m_params;
}