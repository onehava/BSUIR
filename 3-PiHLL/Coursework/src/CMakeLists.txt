cmake_minimum_required(VERSION 3.26)
project(Basic)

set(CMAKE_CXX_STANDARD 20)

add_executable(Basic
        main.cpp
        Constexpr.cpp
        Convert.cpp
        Core.cpp
        Error.cpp
        Evaluator.cpp
        Expression.cpp
        Interpreter.cpp
        Lexeme.cpp
        Lexer.cpp
        Monitor.cpp
        Parser.cpp
        Registry.cpp
        Source.cpp
        Statement.cpp
        Value.cpp)
