#ifndef WORKSHOP10_SHAPE_H
#define WORKSHOP10_SHAPE_H

#include <numbers>

class Point {
    private:
    double m_x;
    double m_y;

    public:
    const double& x;
    const double& y;

    Point(double, double);
};

class Vec2 {
    private:
    Point m_a;
    Point m_b;

    public:
    const double& length;
    const Point& a;
    const Point& b;

    Vec2(Point, Point);
    constexpr Vec2& operator+=(Vec2& rhs);
};


class Shape{
    private:

    public:

    Shape( const double& a, const double& c);

    const double& area;
    const double& circumference;
};

class Circle : public Shape {
    private:
    const double m_radius;
    const double m_area;
    const double m_circumference;

    public:
    Circle(double r);
};

class Parallelogram : public Shape {
    private:
    const double m_height;
    const double m_width;
    const double m_area;
    const double m_corner_angle;
    const double m_circumference;

    public:
    Parallelogram(double side);
    Parallelogram(double side, double theta);
    Parallelogram(double height, double width);
    Parallelogram(double height, double width, double theta);
};

class Rectangle : public Parallelogram {
    public:
    Rectangle(double side);
    Rectangle(double height, double width);
};

class Triangle : public Shape {
    private:
    Point m_a;
    Point m_b;
    Point m_c;
    Vec2 m_AB;
    Vec2 m_BC;
    Vec2 m_CA;
    double m_area;
    double m_circumference;

    public:
    Triangle(Point, Point, Point);
};

#endif