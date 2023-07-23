#include "exception.h"

#include <iostream>

// Basic exception implementation

Exception::Exception(int code, const std::string &message){
    m_code = code;
    m_message = message;
}

// Input exception implementation

InputException::InputException(int code, const std::string &message) : Exception(code, message){}

void InputException::print(){
    std::cerr << "Input exception " << m_code << ": " << m_message << std::endl;
}

// Limit exception implementation

LimitException::LimitException(int code, const std::string &message) : Exception(code, message){}

void LimitException::print(){
    std::cerr << "Limit exception " << m_code << ": " << m_message << std::endl;
}