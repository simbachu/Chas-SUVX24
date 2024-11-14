#include <cmath>
#include <numbers>
#include "Shapes.h"

int main(){

    Point a { 0.0, 0.0 };
    Point b { 1.0, 0.0 };
    Point c (   1.0 * std::cos( 60 * (std::numbers::pi / 180) ),
                1.0 * std::sin( 60 * (std::numbers::pi / 180) ) 
            );


    Circle unit_circle {1};
    Triangle unit_triangle (a, b, c);
    Rectangle unit_rectangle(1);
    Parallelogram unit_diamond(1, 45);

    std::cout << 1 * std::cos(60 * (std::numbers::pi / 180)) << " " << 1 * sin(60 * (std::numbers::pi / 180)) << std::endl;
    std::cout << "Unit circle: Area: " << unit_circle.area << " Perimeter: " << unit_circle.perimeter << std::endl; 
    std::cout << "Unit triangle: Area: " << unit_triangle.area << " Perimeter: " << unit_triangle.perimeter << std::endl; 
    std::cout << "Unit square: Area: " << unit_rectangle.area << " Perimeter: " << unit_rectangle.perimeter << std::endl; 
    std::cout << "Unit rhombus: Area: " << unit_diamond.area << " Perimeter: " << unit_diamond.perimeter << std::endl; 

}