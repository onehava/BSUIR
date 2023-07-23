#pragma once

#include <cstdint>
#include <variant>
#include <string>

class Value {
public:
    enum Type : uint8_t {
        Numeric, Boolean, Literal,
    };

    Value(bool v);
    Value(double v);
    Value(std::string v);

    Type get_type() const;

    bool to_boolean() const;
    double to_numeric() const;
    const std::string& to_literal() const;

private:
    Type m_type;
    std::variant<bool, double, std::string> m_content;
};

std::string_view to_string_view(Value::Type type);