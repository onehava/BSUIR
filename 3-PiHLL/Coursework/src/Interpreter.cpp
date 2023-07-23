#include <iostream>
#include <limits>
#include "Interpreter.h"
#include "Error.h"

Interpreter::Interpreter() : evaluator(registry){}

void Interpreter::execute(const Statements &statements){
    for(auto& statement : statements){
        execute(statement);
    }
}

void Interpreter::execute(const Statement &stmt){
    auto type = stmt.type();

    switch(type){
        case Statement::Type::If: execute((If&) stmt); break;
        case Statement::Type::For: execute((For&) stmt); break;
        case Statement::Type::While: execute((While&) stmt); break;
        case Statement::Type::Assign: execute((Assign&) stmt); break;
        case Statement::Type::Input: execute((Input&) stmt); break;
        case Statement::Type::Print: execute((Print&) stmt); break;
    }
}

void Interpreter::execute(const If &stmt){
    auto& condition = stmt.condition();
    auto v = evaluator.evaluate(condition);

    if(v.to_boolean()){
        execute(stmt.if_branch());
    }
    else{
        execute(stmt.else_branch());
    }
}

void Interpreter::execute(const For &stmt){
    auto& start = stmt.start();
    auto& end = stmt.end();
    auto& ident = stmt.ident();
    auto& body = stmt.body();

    registry[ident] = evaluator.evaluate(start);

    do{
        auto first = registry[ident];
        auto second = evaluator.evaluate(end);

        auto a = first.to_numeric();
        auto b = second.to_numeric();

        if(a <= b){
            execute(body);
            registry[ident] = a + 1;
        }
        else break;
    }
    while(true);
}

void Interpreter::execute(const While &stmt){
    auto& expr = stmt.condition();
    auto& body = stmt.body();

    do{
        auto condition = evaluator.evaluate(expr);
        auto v = condition.to_boolean();

        if(v){
            execute(body);
        }
        else break;
    }
    while(true);
}

void Interpreter::execute(const Assign &stmt){
    auto& ident = stmt.ident();
    auto& expr = stmt.expression();
    registry[ident] = evaluator.evaluate(expr);
}

void Interpreter::execute(const Input &stmt){
    auto& message = stmt.message();
    auto& ident = stmt.ident();

    std::cout << (message.empty() ? "? " : message);
    double input;
    std::cin >> input;

    if(std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    registry[ident] = input;
}

void Interpreter::execute(const Print &stmt){
    auto& params = stmt.params();

    for(auto& param : params){
        auto output = evaluator.evaluate(param);

        switch(output.get_type()){
            case Value::Numeric: std::cout << output.to_numeric(); break;
            case Value::Boolean: std::cout << output.to_boolean(); break;
            case Value::Literal: std::cout << output.to_literal(); break;
        }
    }

    std::cout << std::endl;
}