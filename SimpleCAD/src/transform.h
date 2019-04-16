#pragma once
#include "math.h"

struct Transform
{
	Point translation;
	float rotation;
	Point scale;
};

Point TransformForward(const Transform& transform, Point point);
Point TransformInverse(const Transform& transform, Point point);
