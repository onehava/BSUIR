#pragma once

#include <string>

class Core {
public:
    static void run(const std::string& directory);

private:
    static void run_program(std::string&& program);
};