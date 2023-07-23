#pragma once

#include "Expression.h"
#include "Statement.h"
#include "Lexeme.h"
#include "Constexpr.h"

using Lexemas = std::vector<Lexeme>;

class Parser {
public:
    Parser() = default;
    Statements parse(Lexemas::iterator source);

private:
    Lexemas::iterator m_current;

    template<typename... Types>
    bool match(Types... types);

    inline const Lexeme& previous() const;
    inline const Lexeme& current() const;

    inline void require(Lexeme::Type type);

    void parse_compound(Statements& statements);

    Statement_ptr parse_statement();
    std::unique_ptr<If> parse_if();
    std::unique_ptr<For> parse_for();
    std::unique_ptr<While> parse_while();
    std::unique_ptr<Assign> parse_assign();
    std::unique_ptr<Input> parse_input();
    std::unique_ptr<Print> parse_print();

    Expression_ptr parse_expression(Value::Type type);

    Constexpr expression();

    Constexpr disjunction();
    Constexpr conjunction();

    Constexpr comparison();

    Constexpr term();
    Constexpr factor();

    Constexpr unary();
    Constexpr primary();
};