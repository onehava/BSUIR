#include <iostream>
#include <limits>
#include <cmath>

class Circle
{
public:
    Circle();
    explicit Circle(double radius);

    void set_radius(double radius);
    double get_radius() const;

    double circumference() const;
    double area() const;

    void print() const;

private:
    double m_radius = 0;
};

Circle::Circle() = default;

Circle::Circle(double radius) : m_radius(radius){}

void Circle::set_radius(double radius)
{
    m_radius = radius > 0 ? radius : 0;
}

double Circle::get_radius() const
{
    return m_radius;
}

double Circle::circumference() const
{
    return 2 * M_PI * m_radius;
}

double Circle::area() const
{
    return M_PI * pow(m_radius, 2);
}

void Circle::print() const
{
    std::cout << "Radius: " << m_radius << '\n' << "Circumference: " << circumference() << '\n' << "Area: " << area() << std::endl;
}

int main()
{
    Circle circle;
    double radius;

    do
    {
        std::cout << "Enter the radius of Circle: ";
        std::cin >> radius;

        if(std::cin.fail())
        {
            std::cout << "Invalid input: double excepted\n" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if(radius <= 0)
        {
            std::cout << "Invalid input: radius must be positive\n" << std::endl;
        }
        else
        {
            circle.set_radius(radius);
            break;
        }
    }
    while(true);

    circle.print();
}