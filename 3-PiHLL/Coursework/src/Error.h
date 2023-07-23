#pragma once

#include <exception>
#include "Lexeme.h"
#include "Value.h"

class Error : public std::exception {};

class SyntaxError : public Error {
public:
    SyntaxError(std::size_t row, std::size_t pos);
    std::size_t get_row() const;
    std::size_t get_pos() const;

private:
    std::size_t m_row;
    std::size_t m_pos;
};

class UnknownSymbolError : public SyntaxError {
public:
    UnknownSymbolError(std::size_t row, std::size_t pos, char symbol);
    char get_symbol() const;

private:
    char m_symbol;
};

class UnterminatedStringError : public SyntaxError {
public:
    UnterminatedStringError(std::size_t row, std::size_t pos);
};

class UnexpectedLexemeError : public SyntaxError {
public:
    UnexpectedLexemeError(Lexeme unexpected,
                          Lexeme::Type expected = Lexeme::Unknown);

    Lexeme::Type get_expected() const;
    Lexeme::Type get_unexpected() const;

private:
    Lexeme::Type m_expected;
    Lexeme::Type m_unexpected;
};

class TypeError : public Error {};

class WrongExprTypeError : public TypeError {
public:
    WrongExprTypeError(Lexeme first, Lexeme last,
                       Value::Type unexpected, Value::Type expected);

    Lexeme get_first() const;
    Lexeme get_last() const;
    Value::Type get_expected() const;
    Value::Type get_unexpected() const;

private:
    Lexeme m_first, m_last;
    Value::Type m_expected, m_unexpected;
};

class UnknownUnaryExprError : public TypeError {
public:
    UnknownUnaryExprError(Lexeme op, Value::Type type);
    Lexeme get_operation() const;
    Value::Type get_type() const;

private:
    Lexeme m_op;
    Value::Type m_type;
};

class UnknownBinaryExprError : public TypeError {
public:
    UnknownBinaryExprError(Lexeme op, Value::Type left, Value::Type right);
    Lexeme get_operation() const;
    Value::Type get_left_type() const;
    Value::Type get_right_type() const;

private:
    Lexeme m_op;
    Value::Type m_left, m_right;
};