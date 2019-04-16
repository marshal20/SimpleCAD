#include "transform.h"
#include <math.h>

vec2f TransformForward(const Transform& transform, const vec2f& point)
{
	vec2f new_point = point + transform.translation;
	new_point = { new_point.x * (float)cos(transform.rotation) - new_point.y * (float)sin(transform.rotation) ,
		new_point.x* (float)sin(transform.rotation) + new_point.y * (float)cos(transform.rotation) };
	new_point = new_point * transform.scale;
	return new_point;
}

vec2f TransformInverse(const Transform& transform, const vec2f& point)
{
	vec2f new_point = point / transform.scale;
	new_point = { new_point.x * (float)cos(-transform.rotation) - new_point.y * (float)sin(-transform.rotation) ,
		new_point.x * (float)sin(-transform.rotation) + new_point.y * (float)cos(-transform.rotation) };
	new_point = new_point - transform.translation;
	return new_point;
}
