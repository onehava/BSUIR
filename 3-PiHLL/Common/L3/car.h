//
// Created by Inqognitoo on 11.10.2022.
//

#ifndef LABORATORY_CAR_H
#define LABORATORY_CAR_H

#include <ostream>

struct Display
{
    virtual void print() = 0;
};

class Engine : virtual public Display
{
public:
    enum Type
    {
        Petrol,
        Diesel,
        Hybrid,
        Unmark,
    };

    Engine(Type type, double volume, int power);

    Type get_type() const;
    void set_type(Type type);

    double get_volume() const;
    void set_volume(double volume);

    int get_power() const;
    void set_power(int power);

    void print() override;

private:
    Type m_type;
    double m_volume;
    int m_power;
};

std::ostream& operator<<(std::ostream& ostream, const Engine::Type& type);
std::istream& operator>>(std::istream& istream, Engine::Type& type);

class Body : virtual public Display
{
public:
    Body(int door_count, int length, int width, int height);

    int get_door_count() const;
    void set_door_count(int door_count);

    int get_length() const;
    int get_width() const;
    int get_height() const;

    void set_length(int length);
    void set_width(int width);
    void set_height(int height);

    void print() override;

private:
    int m_door_count;
    int m_dimensions[3];
};

class Color : virtual public Display
{
public:
    Color(const char* color);
    ~Color();

    const char* get_color() const;
    void set_color(const char* color);

    void print() override;

private:
    char* m_color;
};

class Car : public Engine, public Body, public Color
{
public:
    Car(const char* brand, const char* color, int door_count, int length, int width, int height, Engine::Type type, double volume, int power);
    ~Car();

    const char* get_brand() const;
    void set_brand(const char* brand);

    void print() override;

private:
    char* m_brand;
};


#endif //LABORATORY_CAR_H
