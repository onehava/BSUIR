#include "Evaluator.h"
#include "Error.h"

Evaluator::Evaluator(Registry &registry) : m_registry(registry){}

Value Evaluator::evaluate(const Expression &expr){
    auto type = expr.type();

    switch(type){
        case Expression::Type::Unary: return evaluate((Unary&) expr);
        case Expression::Type::Binary: return evaluate((Binary&) expr);
        case Expression::Type::Numeric: return evaluate((Numeric&) expr);
        case Expression::Type::String: return evaluate((String&) expr);
        case Expression::Type::Ident: return evaluate((Ident&) expr);
    }

    throw std::runtime_error(
            "Fatal error: Evaluator::evaluate(const Expression &unary)");
}

Value Evaluator::evaluate(const Unary &unary){
    Value cv = evaluate(unary.expression());

    switch(unary.operation()){
        case Lexeme::Plus:          return {+cv.to_numeric()};
        case Lexeme::Minus:         return {-cv.to_numeric()};
        case Lexeme::Not:           return {!cv.to_boolean()};
        default: break;
    }

    throw std::runtime_error(
            "Fatal error: Evaluator::evaluate(const Unary &unary)");
}

Value Evaluator::evaluate(const Binary &binary){
    Value lv = evaluate(binary.left());
    Value rv = evaluate(binary.right());

    switch(binary.operation()){
        case Lexeme::Plus:          return {lv.to_numeric() + rv.to_numeric()};
        case Lexeme::Minus:         return {lv.to_numeric() - rv.to_numeric()};
        case Lexeme::Asterisk:      return {lv.to_numeric() * rv.to_numeric()};
        case Lexeme::Slash:         return {lv.to_numeric() / rv.to_numeric()};

        case Lexeme::Equals:        return {lv.to_numeric() == rv.to_numeric()};
        case Lexeme::NotEquals:     return {lv.to_numeric() != rv.to_numeric()};
        case Lexeme::Less:          return {lv.to_numeric()  < rv.to_numeric()};
        case Lexeme::LessEquals:    return {lv.to_numeric() <= rv.to_numeric()};
        case Lexeme::Greater:       return {lv.to_numeric()  > rv.to_numeric()};
        case Lexeme::GreaterEquals: return {lv.to_numeric() >= rv.to_numeric()};

        case Lexeme::Or:            return {lv.to_boolean() || rv.to_boolean()};
        case Lexeme::And:           return {lv.to_boolean() && rv.to_boolean()};

        default: break;
    }

    throw std::runtime_error(
            "Fatal error: Evaluator::evaluate(const Binary &binary)");
}

Value Evaluator::evaluate(const Numeric &numeric){
    auto v = numeric.value();
    return {v};
}

Value Evaluator::evaluate(const String &string){
    auto v = string.value();
    return {std::move(v)};
}

Value Evaluator::evaluate(const Ident &ident){
    auto &name = ident.name();
    return m_registry[name];
}