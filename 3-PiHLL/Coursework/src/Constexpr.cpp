#include "Constexpr.h"
#include "Convert.h"

Expression_ptr Constexpr::get_expression(){
    return std::move(m_expr);
}

Value::Type Constexpr::get_type() const {
    return m_type;
}

Constexpr::Constexpr(Expression_ptr expr, Value::Type type){
    m_type = type;
    m_expr = std::move(expr);
}

Constexpr::Constexpr(Lexeme::Type op, Constexpr a, Value::Type type){
    m_type = type;
    m_expr = std::make_unique<Unary>(op, std::move(a.m_expr));
}

Constexpr::Constexpr(Lexeme::Type op, Constexpr a, Constexpr b,
                     Value::Type type){
    m_type = type;
    m_expr = std::make_unique<Binary>(op,
                                      std::move(a.m_expr), std::move(b.m_expr));
}

Constexpr Constexpr::build(Lexeme lexeme){
    auto type = lexeme.get_type();

    switch(type){
        case Lexeme::Ident:
            return {std::make_unique<Ident>(Convert::to_ident(lexeme)),
                    Value::Numeric};
        case Lexeme::String:
            return {std::make_unique<String>(Convert::to_string(lexeme)),
                    Value::Literal};
        case Lexeme::Integer:
        case Lexeme::Floating:
            return {std::make_unique<Numeric>(Convert::to_numeric(lexeme)),
                    Value::Numeric};
        default: break;
    }

    throw UnexpectedLexemeError(lexeme);
}

Constexpr Constexpr::build(Lexeme lexeme, Constexpr a){
    auto vt = a.m_type;
    auto op = lexeme.get_type();

    if(vt == Value::Numeric){
        switch(op){
            case Lexeme::Plus:
            case Lexeme::Minus:
                return {op, std::move(a), Value::Numeric};
            default: break;
        }
    }
    else if(vt == Value::Boolean){
        switch(op){
            case Lexeme::Not: return {op, std::move(a), Value::Boolean};
            default: break;
        }
    }

    throw UnknownUnaryExprError(lexeme, a.m_type);
}

Constexpr Constexpr::build(Lexeme lexeme, Constexpr a, Constexpr b){
    auto at = a.m_type;
    auto bt = b.m_type;
    auto op = lexeme.get_type();

    if(at == Value::Numeric && bt == Value::Numeric){
        switch(op){
            case Lexeme::Plus:
            case Lexeme::Minus:
            case Lexeme::Asterisk:
            case Lexeme::Slash:
                return {op, std::move(a), std::move(b),
                        Value::Numeric};
            case Lexeme::Equals:
            case Lexeme::NotEquals:
            case Lexeme::Less:
            case Lexeme::LessEquals:
            case Lexeme::Greater:
            case Lexeme::GreaterEquals:
                return {op, std::move(a), std::move(b),
                        Value::Boolean};
            default: break;
        }
    }
    else if(at == Value::Boolean && bt == Value::Boolean){
        switch(op){
            case Lexeme::Or:
            case Lexeme::And:
                return {op, std::move(a), std::move(b),
                        Value::Boolean};
            default: break;
        }
    }

    throw UnknownBinaryExprError(lexeme, at, bt);
}