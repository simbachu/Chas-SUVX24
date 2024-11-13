#include <cmath>
#include "Shapes.h"

double d_to_r(double deg){
    return deg * (std::numbers::pi / 180);
}

Point::Point(double x, double y) : 
    m_x{x}, m_y{y}, x{m_x}, y{m_y}{

}

Vec2::Vec2(Point a, Point b) : m_a{a}, m_b{b}, 
    a{m_a}, b{m_b},
    length{ std::abs(
                std::sqrt(
                    std::pow(b.x - a.x, 2.0) + 
                    std::pow(b.y - a.y, 2.0)) 
        )} {

}

Vec2 operator+(const Point& lhs, const Point& rhs){
    return Vec2{lhs, rhs};
}


Shape::Shape(const double& a, const double& c ) : area{a}, circumference{c} {}


Circle::Circle(double r) : m_radius{r}, 
    m_area{(r * r) * std::numbers::pi },
    m_circumference{r * (std::numbers::pi * 2)},
    Shape(m_area, m_circumference)
     {

}

Parallelogram::Parallelogram(double side) : 
    m_height{side}, m_width{side}, m_corner_angle{90},
    m_area{m_height*m_width}, m_circumference{ 
        (m_area / ( m_width * std::sin(d_to_r(180-m_corner_angle))) * 2 + ( m_width * 2 ) )
     },
    Shape(m_area, m_circumference){

}

Triangle::Triangle(Point a, Point b, Point c) : 
    m_a{a}, m_b{b}, m_c{c},
    m_AB{a, b}, m_BC{b, c}, m_CA{c, a},
    m_circumference{m_AB.length + m_BC.length + m_CA.length},
    m_area{ sqrt( (m_circumference/2) *   // Heron's formula
        (m_circumference/2 - m_AB.length) *
        (m_circumference/2 - m_BC.length) *
        (m_circumference/2 - m_CA.length) 
    )}, Shape(m_area, m_circumference)
 {

}