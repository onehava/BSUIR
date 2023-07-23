#include "Convert.h"
#include <charconv>

double Convert::to_numeric(Lexeme lexeme){
    auto view = lexeme.get_view();
    double temp;
    std::from_chars(view.begin(), view.end(), temp);
    return temp;
}

std::string Convert::to_ident(Lexeme lexeme){
    auto view = lexeme.get_view();
    return std::string(view);
}

std::string Convert::to_string(Lexeme lexeme){
    auto view = lexeme.get_view();
    return {std::next(view.begin()), std::prev(view.end())};
}