#include "transform.h"
#include <math.h>

Point TransformForward(const Transform& transform, Point point)
{
	point.x += transform.translation.x;
	point.y += transform.translation.y;
	Point new_point;
	new_point.x = point.x * cos(transform.rotation) - point.y * sin(transform.rotation);
	new_point.y = point.x * sin(transform.rotation) + point.y * cos(transform.rotation);
	new_point.x *= transform.scale.x;
	new_point.y *= transform.scale.y;
	return new_point;
}

Point TransformInverse(const Transform& transform, Point point)
{
	point.x /= transform.scale.x;
	point.y /= transform.scale.y;
	Point new_point;
	new_point.x = point.x * cos(-transform.rotation) - point.y * sin(-transform.rotation);
	new_point.y = point.x * sin(-transform.rotation) + point.y * cos(-transform.rotation);
	new_point.x -= transform.translation.x;
	new_point.y -= transform.translation.y;
	return new_point;
}
