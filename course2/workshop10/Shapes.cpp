#include <cmath>
#include <numbers>
#include "Shapes.h"

double d_to_r(double deg){
    return deg * (std::numbers::pi / 180);
}

Point::Point(double x, double y) : 
    m_x{x}, m_y{y}, x{m_x}, y{m_y}{

}

Vec2::Vec2(Point a, Point b) : m_a{a}, m_b{b}, 
    a{m_a}, b{m_b},
    m_length{ std::abs(
                std::sqrt(
                    std::pow(b.x - a.x, 2.0) + 
                    std::pow(b.y - a.y, 2.0)) 
        )}, length{m_length} {

}

Vec2 operator+(const Point& lhs, const Point& rhs){
    return Vec2{lhs, rhs};
}


Shape::Shape(const double& a, const double& c ) : area{a}, perimeter{c} {}

std::ostream& Shape::print(std::ostream& out) const{
    out << "Area: " << this->area << "Perimeter: " << this->perimeter;
    return out;
}

std::ostream& operator << (std::ostream& out, const Shape& shape){
    return shape.print(out);
}

Circle::Circle(double r_in) : m_radius{r_in}, 
    m_area((r_in * r_in) * std::numbers::pi),
    m_perimeter(r_in * (std::numbers::pi * 2)),
    Shape(m_area, m_perimeter)
     {

}

std::ostream& Circle::print(std::ostream& out) const {
    return Shape::print(out);
}

Parallelogram::Parallelogram(double a, double theta) : 
    m_side_a{a}, m_side_b{a}, m_corner_angle{theta},
    m_area{m_side_a*m_side_b}, 
    m_perimeter{ 
        (m_area / ( m_side_a * std::sin(d_to_r(180-theta))) * 2 + ( m_side_b * 2 ) )
     },
    Shape(m_area, m_perimeter){
}

Parallelogram::Parallelogram(double a, double b, double theta) : 
    m_side_a{a}, m_side_b{b}, m_corner_angle{theta},
    m_area{m_side_a*m_side_b * std::sin(d_to_r(180-theta))}, 
    m_perimeter{ 
        (m_area / ( m_side_a * std::sin(d_to_r(180-theta))) * 2 + ( m_side_b * 2 ) )
     },
    Shape(m_area, m_perimeter){
}

Rectangle::Rectangle(double side) : 
    Parallelogram(side, 90){

}

Rectangle::Rectangle(double height, double width) : 
    Parallelogram(height, width, 90){

}

Triangle::Triangle(Point a, Point b, Point c) : 
    m_AB{a, b}, m_BC{b, c}, m_CA{c, a},
    m_perimeter{m_AB.length + m_BC.length + m_CA.length},
    m_area( //heron's formula
        std::sqrt( (m_perimeter/2.0) * (m_perimeter/2.0 - m_AB.length) * (m_perimeter/2.0 - m_BC.length) * (m_perimeter/2.0 - m_CA.length) )
    ),
    Shape(m_area, m_perimeter)
 {

}