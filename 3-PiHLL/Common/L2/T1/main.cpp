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

    friend std::ostream& operator<<(std::ostream& stream, const Circle& circle);

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

std::ostream& operator<<(std::ostream &stream, const Circle& circle)
{
    return stream << "Radius: " << circle.m_radius << '\n'
                  << "Circumference: " << circle.circumference() << '\n'
                  << "Area: " << circle.area() << std::endl;
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

    std::cout << circle;
}