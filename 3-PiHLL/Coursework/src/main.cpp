#include <iostream>
#include "Core.h"

int main(int argc, char** argv){
    if(argc == 2){
        Core::run(argv[1]);
    }
    else {
        std::cerr << "usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }
}
