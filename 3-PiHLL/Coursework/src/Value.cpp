#include "Value.h"

Value::Value(bool v){
    m_type = Type::Boolean;
    m_content = v;
}

Value::Value(double v){
    m_type = Type::Numeric;
    m_content = v;
}

Value::Value(std::string v){
    m_type = Type::Literal;
    m_content = std::move(v);
}

Value::Type Value::get_type() const {
    return m_type;
}

bool Value::to_boolean() const {
    return std::get<bool>(m_content);
}

double Value::to_numeric() const {
    return std::get<double>(m_content);
}

const std::string &Value::to_literal() const {
    return std::get<std::string>(m_content);
}

std::string_view to_string_view(Value::Type type){
    static const char* const conversion[] = {
            "Numeric", "Boolean", "Literal",
    };

    return conversion[type];
}