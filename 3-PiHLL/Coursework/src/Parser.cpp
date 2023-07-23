#include "Parser.h"
#include "Convert.h"
#include "Error.h"
#include <algorithm>
#include <memory>

Statements Parser::parse(Lexemas::iterator source){
    m_current = source;

    Statements stmts;
    parse_compound(stmts);
    require(Lexeme::Eof);

    return stmts;
}

template<typename ...Types>
bool Parser::match(Types... types){
    auto unpack = {types...};
    auto search = m_current->get_type();

    if(std::ranges::any_of(unpack,
                           [search](auto type){ return type == search; })){
        ++m_current;
        return true;
    }

    return false;
}

const Lexeme &Parser::previous() const {
    return *(m_current - 1);
}

const Lexeme &Parser::current() const {
    return *m_current;
}

void Parser::require(Lexeme::Type type){
    if(!match(type)){
        throw UnexpectedLexemeError(*m_current, type);
    }
}

void Parser::parse_compound(Statements& statements){
    Statement_ptr stmt;
    do{
        while(match(Lexeme::Eol));

        stmt = parse_statement();

        if(stmt){
            statements.push_back(std::move(stmt));
            require(Lexeme::Eol);
        }
        else break;
    }
    while(true);
}

Statement_ptr Parser::parse_statement(){
    auto type = m_current->get_type();

    switch(type){
        case Lexeme::If: return parse_if();
        case Lexeme::For: return parse_for();
        case Lexeme::While: return parse_while();

        case Lexeme::Let:
        case Lexeme::Ident: return parse_assign();

        case Lexeme::Input: return parse_input();
        case Lexeme::Print: return parse_print();

        default: return nullptr;
    }
}

std::unique_ptr<If> Parser::parse_if(){
    require(Lexeme::If);
    auto expr = parse_expression(Value::Boolean);

    require(Lexeme::Then);
    Statements if_branch, else_branch;

    if(match(Lexeme::Eol)){
        parse_compound(if_branch);

        if(match(Lexeme::Else)){
            parse_compound(else_branch);
        }

        require(Lexeme::Endif);
    }
    else{
        Statement_ptr stmt = parse_statement();
        if_branch.push_back(std::move(stmt));
    }

    return std::make_unique<If>(std::move(expr),
                                std::move(if_branch), std::move(else_branch));
}

std::unique_ptr<For> Parser::parse_for(){
    require(Lexeme::For);

    require(Lexeme::Ident);
    auto ident = Convert::to_ident(previous());

    require(Lexeme::Equals);
    auto start = parse_expression(Value::Numeric);

    require(Lexeme::To);
    auto end = parse_expression(Value::Numeric);

    require(Lexeme::Eol);
    Statements body;
    parse_compound(body);

    require(Lexeme::Next);

    return std::make_unique<For>(std::move(ident),
                                 std::move(start), std::move(end), std::move(body));
}

std::unique_ptr<While> Parser::parse_while(){
    require(Lexeme::While);
    auto expr = parse_expression(Value::Boolean);

    require(Lexeme::Eol);
    Statements body;
    parse_compound(body);

    require(Lexeme::Wend);

    return std::make_unique<While>(std::move(expr), std::move(body));
}

std::unique_ptr<Assign> Parser::parse_assign(){
    match(Lexeme::Let);
    require(Lexeme::Ident);
    auto ident = Convert::to_ident(previous());

    require(Lexeme::Equals);
    auto expr = parse_expression(Value::Numeric);

    return std::make_unique<Assign>(std::move(ident), std::move(expr));
}

std::unique_ptr<Input> Parser::parse_input(){
    require(Lexeme::Input);
    std::string message, ident;

    if(match(Lexeme::String)){
        message = Convert::to_string(previous());
        require(Lexeme::Comma);
    }

    require(Lexeme::Ident);
    ident = Convert::to_ident(previous());

    return std::make_unique<Input>(std::move(message), std::move(ident));
}

std::unique_ptr<Print> Parser::parse_print(){
    require(Lexeme::Print);
    Expressions params;

    do{
        auto cexpr = expression();
        auto expr = cexpr.get_expression();
        params.push_back(std::move(expr));
    }
    while(match(Lexeme::Semicolon));

    return std::make_unique<Print>(std::move(params));
}

Expression_ptr Parser::parse_expression(Value::Type type){
    auto first = current();

    auto cexpr = expression();
    auto ctype = cexpr.get_type();

    if(ctype == type){
        return cexpr.get_expression();
    }

    auto last = previous();

    throw WrongExprTypeError(first, last, ctype, type);
}


Constexpr Parser::expression(){
    return disjunction();
}

Constexpr Parser::disjunction(){
    auto a = conjunction();

    while(match(Lexeme::Or)){
        auto op = previous();
        auto b = conjunction();
        a = Constexpr::build(op, std::move(a), std::move(b));
    }

    return a;
}

Constexpr Parser::conjunction(){
    auto a = comparison();

    while(match(Lexeme::And)){
        auto op = previous();
        auto b = comparison();
        a = Constexpr::build(op, std::move(a), std::move(b));
    }

    return a;
}

Constexpr Parser::comparison(){
    auto a = term();

    if(match(Lexeme::Equals, Lexeme::NotEquals,
             Lexeme::Less, Lexeme::LessEquals,
             Lexeme::Greater, Lexeme::GreaterEquals)){
        auto op = previous();
        auto b = term();
        return Constexpr::build(op, std::move(a), std::move(b));
    }

    return a;
}

Constexpr Parser::term(){
    auto a = factor();

    while(match(Lexeme::Plus, Lexeme::Minus)){
        auto op = previous();
        auto b = factor();
        a =  Constexpr::build(op, std::move(a), std::move(b));
    }

    return a;
}

Constexpr Parser::factor(){
    auto a = unary();

    while(match(Lexeme::Asterisk, Lexeme::Slash)){
        auto op = previous();
        auto b = unary();
        a = Constexpr::build(op, std::move(a), std::move(b));
    }

    return a;
}

Constexpr Parser::unary(){
    if(match(Lexeme::Plus, Lexeme::Minus, Lexeme::Not)){
        auto op = previous();
        auto e = primary();
        return Constexpr::build(op, std::move(e));
    }

    return primary();
}

Constexpr Parser::primary(){
    auto lexeme = current();

    if(match(Lexeme::Floating, Lexeme::Integer,
             Lexeme::String, Lexeme::Ident)){
        return Constexpr::build(lexeme);
    }

    if(match(Lexeme::OpenParen)){
        auto e = expression();
        require(Lexeme::CloseParen);
        return e;
    }

    throw UnexpectedLexemeError(lexeme);
}