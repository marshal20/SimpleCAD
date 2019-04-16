#pragma once
#include "vector.h"

struct Transform
{
	vec2f translation;
	float rotation;
	vec2f scale;
};

vec2f TransformForward(const Transform& transform, const vec2f& point);
vec2f TransformInverse(const Transform& transform, const vec2f& point);
