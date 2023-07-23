#pragma once

#include "Source.h"
#include "Lexeme.h"
#include <vector>

using Lexemas = std::vector<Lexeme>;

class Lexer {
public:
    Lexer() = default;
    Lexemas tokenize(Source::iterator source);

private:
    Source::iterator m_curr;
    Source::iterator m_prev;

    bool match(char required);

    Lexeme next();
    Lexeme make(Lexeme::Type type);

    Lexeme::Type read_lexeme();
    Lexeme::Type read_number();
    Lexeme::Type read_string();
    Lexeme::Type read_unique();
};

namespace Character
{
    bool is_digit(char c);
    bool is_alpha(char c);
    bool is_space(char c);
    bool is_ident(char c);
}