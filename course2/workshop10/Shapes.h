#ifndef WORKSHOP10_SHAPE_H
#define WORKSHOP10_SHAPE_H

#include <iostream>
#include <vector>
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
    double m_length;

    public:

    const Point& a;
    const Point& b;
    const double& length;

    Vec2(Point, Point);
    constexpr Vec2& operator+=(Vec2& rhs);
};


class Shape{
    private:

    public:

    Shape( const double& a, const double& c);

    const double& area;
    const double& perimeter;

    virtual std::ostream& print(std::ostream& out) const;
};

class Circle : public Shape {
    private:
    const double m_radius;
    const double m_area;
    const double m_perimeter;

    public:
    Circle(double r);

    virtual std::ostream& print(std::ostream& out) const override;
};

class Parallelogram : public Shape {
    private:
    const double m_side_a;
    const double m_side_b;
    const double m_area;
    const double m_corner_angle;
    const double m_perimeter;

    public:
    Parallelogram(double a, double theta);
    Parallelogram(double a, double b, double theta);
};

class Rectangle : public Parallelogram {
    public:
    Rectangle(double side);
    Rectangle(double height, double width);
};

class Triangle : public Shape {
    private:
    Vec2 m_AB;
    Vec2 m_BC;
    Vec2 m_CA;
    double m_area;
    double m_perimeter;

    public:
    Triangle(Point, Point, Point);
};

#endif