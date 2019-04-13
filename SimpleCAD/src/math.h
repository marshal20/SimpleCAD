#pragma once

#define PI 3.141593f

struct Point
{
	float x, y;
};

struct Line
{
	Point p1, p2;
};

float distance(Point p1, Point p2);
