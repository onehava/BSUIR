#ifndef LABORATORY_EXCEPTION_H
#define LABORATORY_EXCEPTION_H

#include <string>
#include <stdexcept>

class Exception : public std::exception {
public:
    virtual void print() = 0;

protected:
    Exception(int code, const std::string& message);
    int m_code;
    std::string m_message;
};

class InputException : public Exception {
public:
    InputException(int code, const std::string& message);
    void print() override;
};

class LimitException : public Exception {
public:
    LimitException(int code, const std::string& message);
    void print() override;
};

#endif