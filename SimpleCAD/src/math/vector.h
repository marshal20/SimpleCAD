#pragma once
#include <math.h>

template <typename T>
struct vec2
{
	T x, y;

	vec2(const vec2<T>& other) :
		x{ other.x }, y{ other.y }
	{ }

	vec2() :
		x{0}, y{0}
	{ }

	vec2(T scaler) :
		x{ scaler }, y{ scaler }
	{ }

	vec2(T _x, T _y) :
		x{ _x }, y{ _y }
	{ }

	vec2<T> operator+(const vec2<T>& rhs) const
	{
		vec2<T> res;
		res.x = x + rhs.x;
		res.y = y + rhs.y;
		return res;
	}

	vec2<T> operator-(const vec2<T>& rhs) const
	{
		vec2<T> res;
		res.x = x - rhs.x;
		res.y = y - rhs.y;
		return res;
	}

	vec2<T> operator*(const vec2<T>& rhs) const
	{
		vec2<T> res;
		res.x = x * rhs.x;
		res.y = y * rhs.y;
		return res;
	}

	vec2<T> operator/(const vec2<T>& rhs) const
	{
		vec2<T> res;
		res.x = x / rhs.x;
		res.y = y / rhs.y;
		return res;
	}

	vec2<T> operator*(const T& rhs) const
	{
		vec2<T> res;
		res.x = x * rhs;
		res.y = y * rhs;
		return res;
	}

	vec2<T> operator/(const T& rhs) const
	{
		vec2<T> res;
		res.x = x / rhs;
		res.y = y / rhs;
		return res;
	}

	T amplitude() const
	{
		return sqrt(x * x + y * y);
	}

	vec2<T> normalised() const
	{
		return *this / amplitude();
	} 
};

template <typename T>
T dot(const vec2<T>& v1, const vec2<T>& v2)
{
	return v1.x* v2.x + v1.y * v2.y;
}

template <typename T>
T cross(const vec2<T>& v1, const vec2<T>& v2)
{
	return v1.x* v2.y - v1.y * v2.x;
}

template <typename T>
T distance(const vec2<T>& v1, const vec2<T>& v2)
{
	return (float)sqrt((float)pow((v2.x - v1.x), 2) + (float)pow((v2.y - v1.y), 2));
}

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;
