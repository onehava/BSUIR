#pragma once

#include <cstdint>
#include <string_view>

class Lexeme {
public:
    enum Type : uint8_t {
        Floating, Integer, String, Ident, Unknown,
        Comma, Colon, Semicolon,
        Equals, NotEquals, Less, LessEquals,
        Greater, GreaterEquals,
        OpenParen, CloseParen,
        Plus, Minus, Asterisk, Slash,
        And, Or, Not,
        Let,
        Input, Print,
        Goto,
        If, Then, Else, Endif,
        For, To, Step, Next,
        While, Wend,
        Eol,
        Eof,
    };

    Lexeme();
    Lexeme(Type type, std::string_view view,
           std::size_t row, std::size_t pos);

    Type get_type() const;
    bool has_type(Type type) const;

    std::string_view get_view() const;
    std::size_t get_row() const;
    std::size_t get_pos() const;

private:
    Type m_type;
    std::string_view m_view;
    std::size_t m_row{};
    std::size_t m_pos{};
};

Lexeme::Type to_lexeme_type(std::string_view view);

std::string_view to_string_view(Lexeme::Type type);