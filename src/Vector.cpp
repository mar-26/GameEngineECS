#include "../include/Vector.h"

void Vector::invert()
{
	x = -x;
	y = -y;
}

Vector Vector::normalize()
{
	float l = magnitude();
	if (l > 0)
	{
		(*this) *= ((float)1)/l;
	}
	return *this;
}

float Vector::magnitude() const
{
	return sqrt(x*x + y*y);
}

void Vector::addScaledVector(const Vector& v, float scale)
{
	x += v.x * scale;
	y += v.y * scale;
}

void Vector::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
}

void Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
}

void Vector::operator-=(const Vector& v)
{
	x -= v.x;
	y -= v.y;
}

float Vector::operator*(const Vector& v) const
{
	return x*v.x + y*v.y;
}

Vector Vector::operator*(const float scale) const
{
	return Vector(x*scale, y*scale);
}

Vector Vector::operator+(const Vector& v)
{
	return Vector(x+v.x, y+v.y);
}

Vector Vector::operator-(const Vector& v)
{
	return Vector(x-v.x, y-v.y);
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
	os << "<" << v.x << ", " << v.y << ">";
	return os;
}
