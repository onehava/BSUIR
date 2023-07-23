#pragma once

#include "Registry.h"
#include "Expression.h"

class Evaluator {
public:
    Evaluator(Registry& registry);
    Value evaluate(const Expression& expr);

private:
    Registry& m_registry;

    inline Value evaluate(const Unary& unary);
    inline Value evaluate(const Binary& binary);
    inline Value evaluate(const Numeric& numeric);
    inline Value evaluate(const String& string);
    inline Value evaluate(const Ident& ident);
};