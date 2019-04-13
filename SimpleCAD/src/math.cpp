#include "math.h"
#include <math.h>

float distance(Point p1, Point p2)
{
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}
