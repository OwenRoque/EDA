#ifndef __POINT_H__
#define __POINT_H__

struct Point
{
    double x;
    double y;
    double z;
    Point() = default;
    Point(double a, double b, double c) : x(a), y(b), z(c) {}
};

#endif