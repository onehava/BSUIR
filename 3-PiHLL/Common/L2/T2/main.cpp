//
// Created by Inqognitoo on 24.09.2022.
//

#include "my_string.h"

#include <iostream>

void test_memory_allocation()
{
    std::cout << "Memory allocation:" << std::endl;

    String input("123456789");
    String delta("1234567");

    std::cout << "\tinput: length: " << input.length() << " capacity: " << input.capacity() << std::endl;
    input += delta;
    std::cout << "\tdelta: length: " << input.length() << " capacity: " << input.capacity() << std::endl;

    std::cout << std::endl;
}

int main()
{
    test_memory_allocation();

    String a("A very very very very very very long string!");
    String b("A very short string!");

    std::cout << a << ' ' << std::endl;
    std::cout << b << std::endl;

    String c = a + b;

    std::cout << c << std::endl;

    c += String("Something very very very very strange");

    std::cout << c << std::endl;

    String man = "Man";
    String pen = "Pen";

    if(man == pen)
    {
        std::cout << "This man is pen" << std::endl;
    }

    if(man != pen)
    {
        std::cout << "This man is not pen" << std::endl;
    }

    String x = "Hippopotomonstrosesquipedaliophobia";
    String y = x(29, 6);

    std::cout << "Target: " << x << std::endl;
    std::cout << "Substr: " << y << std::endl;

    std::cout << x[0] << x[1] << x[2]
              << x[3] << x[4] << x[5]
              << x[6] << x[7] << x[8] << "..." << std::endl;
}