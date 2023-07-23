#include "Error.h"

SyntaxError::SyntaxError(std::size_t row, std::size_t pos){
    m_row = row;
    m_pos = pos;
}

std::size_t SyntaxError::get_row() const {
    return m_row;
}

std::size_t SyntaxError::get_pos() const {
    return m_pos;
}

UnknownSymbolError::UnknownSymbolError(
        std::size_t row, std::size_t pos, char symbol)
        : SyntaxError(row, pos), m_symbol(symbol){}

char UnknownSymbolError::get_symbol() const {
    return m_symbol;
}

UnterminatedStringError::UnterminatedStringError(
        std::size_t row, std::size_t pos)
        : SyntaxError(row, pos){}

UnexpectedLexemeError::UnexpectedLexemeError(
        Lexeme unexpected, Lexeme::Type expected)
        : SyntaxError(unexpected.get_row(), unexpected.get_pos()),
          m_expected(expected),
          m_unexpected(unexpected.get_type()){}

Lexeme::Type UnexpectedLexemeError::get_expected() const {
    return m_expected;
}

Lexeme::Type UnexpectedLexemeError::get_unexpected() const {
    return m_unexpected;
}

WrongExprTypeError::WrongExprTypeError(Lexeme first, Lexeme last,
                                       Value::Type unexpected, Value::Type expected){
    m_first = first;
    m_last = last;
    m_unexpected = unexpected;
    m_expected= expected;
}

Lexeme WrongExprTypeError::get_first() const {
    return m_first;
}

Lexeme WrongExprTypeError::get_last() const {
    return m_last;
}

Value::Type WrongExprTypeError::get_expected() const {
    return m_expected;
}

Value::Type WrongExprTypeError::get_unexpected() const {
    return m_unexpected;
}

UnknownUnaryExprError::UnknownUnaryExprError(
        Lexeme op, Value::Type type){
    m_op = op;
    m_type = type;
}

Lexeme UnknownUnaryExprError::get_operation() const {
    return m_op;
}

Value::Type UnknownUnaryExprError::get_type() const {
    return m_type;
}

UnknownBinaryExprError::UnknownBinaryExprError(
        Lexeme op, Value::Type left, Value::Type right){
    m_op = op;
    m_left = left;
    m_right = right;
}

Lexeme UnknownBinaryExprError::get_operation() const {
    return m_op;
}

Value::Type UnknownBinaryExprError::get_left_type() const {
    return m_left;
}

Value::Type UnknownBinaryExprError::get_right_type() const {
    return m_right;
}