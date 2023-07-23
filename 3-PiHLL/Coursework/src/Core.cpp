#include "Core.h"
#include "Lexer.h"
#include "Parser.h"
#include "Monitor.h"
#include "Interpreter.h"

#include <fstream>
#include <iostream>

void Core::run(const std::string &directory){
    std::ifstream istream(directory);

    if(!istream.is_open()){
        std::cerr << "Error: unknown directory `"
                  << directory << "`" << std::endl;
        std::exit(65);
    }

    std::string source((std::istreambuf_iterator<char>(istream)),
                       std::istreambuf_iterator<char>());
    istream.close();

    run_program(std::move(source));
}

void Core::run_program(std::string&& program){
    Source source(program);

    Monitor monitor(source, [&]{
        Lexer lexer;
        Lexemas lexemas = lexer.tokenize(source.begin());

        Parser parser;
        Statements statements = parser.parse(lexemas.begin());

        Interpreter interpreter;
        interpreter.execute(statements);
    });

    monitor.run();
}