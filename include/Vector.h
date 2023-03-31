#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>

class Vector
{
	public:
		float x, y;

		Vector() : x(0), y(0) {}
		Vector(float x, float y) : x(x), y(y) {}

		void invert();
		Vector normalize();
		float magnitude() const;
		void addScaledVector(const Vector& v, float scale);
		Vector crossProduct(const Vector& v) const;

		void operator*=(const float scale);
		void operator+=(const Vector& v);
		void operator-=(const Vector& v);

		float operator*(const Vector& v) const;
		Vector operator*(const float scale) const;
		Vector operator+(const Vector& v);
		Vector operator-(const Vector& v);
		friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

#endif
