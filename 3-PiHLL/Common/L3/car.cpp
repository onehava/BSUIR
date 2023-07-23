//
// Created by Inqognitoo on 11.10.2022.
//

#include <algorithm>
#include <cstring>
#include <iostream>

#include "car.h"

#define init(dest, src)             \
    dest = new char[strlen(src)];   \
    strcpy(dest, src);

#define copy(dest, src)             \
    delete dest;                    \
    init(dest, src);

// Engine

Engine::Engine(Engine::Type type, double volume, int power) : m_type(type), m_volume(volume), m_power(power){}

Engine::Type Engine::get_type() const {return m_type;}
void Engine::set_type(Engine::Type type) {m_type = type;}

double Engine::get_volume() const {return m_volume;}
void Engine::set_volume(double volume) {m_volume = volume;}

int Engine::get_power() const {return m_power;}
void Engine::set_power(int power) {m_power = power;}

void Engine::print()
{
    std::cout << "type = " << m_type << '\n'
              << "volume = " << m_volume << '\n'
              << "power = " << m_power
              << std::endl;
}

// Engine::Type friend iostream operators

std::ostream& operator<<(std::ostream& ostream, const Engine::Type& type)
{
    const char* const names[] =
    {
        "petrol",
        "diesel",
        "hybrid",
        "unmark",
    };

    ostream << names[static_cast<int>(type)];
    return ostream;
}

std::istream& operator>>(std::istream& istream, Engine::Type& type)
{
    std::string input;
    istream >> input;

    const char* const names[] =
    {
        "petrol",
        "diesel",
        "hybrid",
    };

    for(int i = 0; i < sizeof(names) / sizeof(const char*); i++)
    {
        if(input == names[i])
        {
            type = static_cast<Engine::Type>(i);
            return istream;
        }
    }

    type = Engine::Type::Unmark;
    return istream;
}

// Body

Body::Body(int door_count, int length, int width, int height) : m_door_count(door_count), m_dimensions{length, width, height}{}

int Body::get_door_count() const {return m_door_count;}
void Body::set_door_count(int door_count) {m_door_count = door_count;}

int Body::get_length() const {return m_dimensions[0];}
int Body::get_width() const {return m_dimensions[1];}
int Body::get_height() const {return m_dimensions[2];}

void Body::set_length(int length) {m_dimensions[0] = length;}
void Body::set_width(int width) {m_dimensions[1] = width;}
void Body::set_height(int height) {m_dimensions[2] = height;}

void Body::print()
{
    std::cout << "door count = " << m_door_count << '\n'
              << "dimensions = " << m_dimensions[0] << ':'
                                 << m_dimensions[1] << ':'
                                 << m_dimensions[2]
              << std::endl;
}

// Color

Color::Color(const char *color)
{
    init(m_color, color);
}

Color::~Color()
{
    delete m_color;
}

const char* Color::get_color() const {return m_color;}
void Color::set_color(const char *color){copy(m_color, color);}

void Color::print()
{
    std::cout << "color = " << m_color
              << std::endl;
}

// Car

Car::Car(const char *brand, const char *color, int door_count, int length, int width, int height, Engine::Type type,
         double volume, int power) : Color(color), Body(door_count, length, width, height), Engine(type, volume, power)
{
    init(m_brand, brand);
}

Car::~Car()
{
    delete m_brand;
}

const char* Car::get_brand() const {return m_brand;}
void Car::set_brand(const char *brand){copy(m_brand, brand);}

void Car::print()
{
    Engine::print();
    Body::print();
    Color::print();
}

#undef copy
#undef init