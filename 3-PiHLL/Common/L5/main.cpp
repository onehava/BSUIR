#include <iostream>
#include <cmath>
#include "list.h"
#include "exception.h"

int main(){
    try{
        std::cout << "This program reads list of normalized "
                     "double-precision floats and "
                     "prints the elements closest to their average" << std::endl;

        std::size_t count;

        std::cout << "Enter count of elements: ";
        std::cin >> count;

        if(std::cin.fail()){
            throw InputException(1, "count of elements should be represented as unsigned integer");
        }

        if(count > 100){
            throw LimitException(1, "count of elements should be no greater than 100");
        }

        List<double> elements;

        for(auto i = 1; i <= count; i++){
            double element;

            std::cout << "Enter " << i << " element: ";
            std::cin >> element;

            if(std::cin.fail()){
                throw InputException(2, "element should be represented as double-precision float");
            }

            if(fabs(element) >= 1){
                throw LimitException(2, "element should be normalized");
            }

            elements.push_back(element);
        }
    }
    catch(Exception& e){
        e.print();
    }
    catch(...){
        std::cerr << "Unexpected exception" << std::endl;
    }

    return 0;
}
