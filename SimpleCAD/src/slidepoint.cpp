#include "slidepoint.h"
#include <stdio.h>

SlidePoint::SlidePoint()
{

}

SlidePoint::~SlidePoint()
{

}

void SlidePoint::start(float x, float y)
{
	is_sliding = true;
	x1 = x;
	y1 = y;
}

void SlidePoint::update(float x, float y)
{
	if (is_sliding)
	{
		x2 = x;
		y2 = y;
	}
}

void SlidePoint::end()
{
	is_sliding = false;
}

bool SlidePoint::issliding()
{
	return is_sliding;
}

float SlidePoint::getx()
{
	return x2;
}

float SlidePoint::gety()
{
	return y2;
}

float SlidePoint::getdeltax()
{
	return x2 - x1;
}

float SlidePoint::getdeltay()
{
	return y2 - y1;
}
