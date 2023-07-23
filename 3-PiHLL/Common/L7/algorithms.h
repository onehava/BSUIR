#ifndef LABORATORY_ALGORITHMS_H
#define LABORATORY_ALGORITHMS_H

#include "list.h"
#include <functional>

template <class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& element){
    for(auto it = first; it != last; ++it){
        if(*it == element){
            return it;
        }
    }
    return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate predicate){
    for(auto it = first; it != last; ++it){
        if(predicate(*it)){
            return it;
        }
    }
    return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate predicate){
    for(auto it = first; it != last; ++it){
        if(!predicate(*it)){
            return it;
        }
    }
    return last;
}

template <class InputIt, class UnaryFunction>
void for_each(InputIt first, InputIt last, UnaryFunction function){
    for(auto it = first; it != last; ++it){
        function(*it);
    }
}

#endif
