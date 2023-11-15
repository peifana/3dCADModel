#pragma once
class Point {
public:
	double x, y, z;
public:
	Point(double x, double y, double z):x(x),y(y),z(z) {};
	Point():x(0),y(0),z(0) {};
};