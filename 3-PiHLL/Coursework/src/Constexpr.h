#pragma once

#include "Expression.h"
#include "Value.h"
#include "Error.h"

class Constexpr {
public:
    static Constexpr build(Lexeme lexeme);
    static Constexpr build(Lexeme lexeme, Constexpr a);
    static Constexpr build(Lexeme lexeme, Constexpr a, Constexpr b);

    Expression_ptr get_expression();
    Value::Type get_type() const;

private:
    Constexpr(Expression_ptr expr, Value::Type type);
    Constexpr(Lexeme::Type op, Constexpr a, Value::Type type);
    Constexpr(Lexeme::Type op, Constexpr a, Constexpr b, Value::Type type);

    Expression_ptr m_expr;
    Value::Type m_type;
};