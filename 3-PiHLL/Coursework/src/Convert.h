#pragma once

#include "Lexeme.h"
#include <string>

namespace Convert {
    double to_numeric(Lexeme lexeme);
    std::string to_ident(Lexeme lexeme);
    std::string to_string(Lexeme lexeme);
}