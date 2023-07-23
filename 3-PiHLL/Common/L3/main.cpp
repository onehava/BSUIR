//
// Created by Inqognitoo on 11.10.2022.
//

#include <iostream>

#include "car.h"

Car input()
{
    char brand[256];
    char color[256];

    int door_count, length, width, height;
    Engine::Type type;
    double volume;
    int power;

    std::cout << "Enter car brand: ";
    std::cin.getline(brand, sizeof(brand));

    std::cout << "Enter car color: ";
    std::cin.getline(color, sizeof(color));

    std::cout << "Enter door count: ";
    std::cin >> door_count;

    std::cout << "Enter car sizes:\n";

    std::cout << "\tLength = ";
    std::cin >> length;

    std::cout << "\tWidth = ";
    std::cin >> width;

    std::cout << "\tHeight = ";
    std::cin >> height;

    std::cout << "Enter engine type: ";
    std::cin >> type;

    std::cout << "Enter engine volume: ";
    std::cin >> volume;

    std::cout << "Enter engine power: ";
    std::cin >> power;

    return {brand, color, door_count, length, width, height, type, volume, power};
}

int main()
{
    Car car = input();
    car.print();
}