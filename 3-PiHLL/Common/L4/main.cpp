#include <iostream>
#include <limits>
#include <vector>

#include "list.h"
#include "my_string.h"

// Debug

template<typename T>
void write(std::ostream& ostream, const List<T>& list){
    auto it = list.begin();

    if(it != list.end()){
        std::cout << *it;

        while(++it != list.end()){
            std::cout << ' ' << *it;
        }
    }

    std::cout << std::endl;
}

template<typename T>
inline void print(const List<T>& list){
    write(std::cout, list);
}

// Tasks

void list_maximum(){
    List<int> ints;

    std::cout << "Enter integers: ";

    while(true){
        int x;
        std::cin >> x;

        if(std::cin.fail()){
            break;
        }
        else{
            ints.push_back(x);
        }
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int max = ints.front();

    for(auto& i : ints){
        max = std::max(i, max);
    }

    std::cout << "Max integer: " << max << std::endl;
}

void list_longest(){
    List<String> strings;

    while(!std::cin.eof()){
        String string;
        std::cin >> string;

        if(string.length()){
            strings.push_back(string);
        }
    }

    String& longest = strings.front();

    for(auto &s : strings){
        if(s.length() > longest.length()){
            longest = s;
        }
    }

    std::cout << "Longest string: " << longest << std::endl;
}

int main(){
    list_maximum();
    list_longest();
}