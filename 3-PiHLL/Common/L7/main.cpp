#include <iostream>
#include <iomanip>
#include <limits>
#include "algorithms.h"

int main(){

    // Dataset initialization

    List<int> dataset = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Find sum of dataset using `for_each`

    auto sum = 0;

    for_each(dataset.begin(), dataset.end(), [&sum](const int& i){
         sum += i;
    });

    std::cout << "Sum of dataset elements is " << sum << std::endl;

    // Find numbers in dataset using `find`

    std::cout << "\nLet's find some numbers!"
                 "\nTo stop search enter any wrong numeric sequence." << std::endl;

    while(true){
        int to_search;
        std::cin >> to_search;

        if(std::cin.fail()){
            break;
        }

        auto iterator = find(dataset.begin(), dataset.end(), to_search);

        if(iterator == dataset.end()){
            std::cout << "Entered number is not found in dataset" << std::endl;
        }
        else{
            std::cout << "Entered number found in dataset" << std::endl;
        }
    }

    // Find even numbers in dataset using `find_if`

    List<int> evens;

    auto current = dataset.begin();

    while(true){
        current = find_if(current, dataset.end(), [](const int& i){return i % 2 == 0;});

        if(current == dataset.end()){
            break;
        }
        else{
            evens.push_back(*current);
            ++current;
        }
    }

    std::cout << "\nEven numbers in dataset: ";

    for_each(evens.begin(), evens.end(), [](const int& i){
        std::cout << i << ' ';
    });

    std::cout << std::endl;
}