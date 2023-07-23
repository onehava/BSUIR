#pragma once

#include "Statement.h"
#include "Expression.h"
#include "Evaluator.h"

class Interpreter {
public:
    Interpreter();
    void execute(const Statements& statements);

private:
    Registry registry;
    Evaluator evaluator;

    void execute(const Statement& stmt);
    inline void execute(const If& stmt);
    inline void execute(const For& stmt);
    inline void execute(const While& stmt);
    inline void execute(const Assign& stmt);
    inline void execute(const Input& stmt);
    inline void execute(const Print& stmt);
};