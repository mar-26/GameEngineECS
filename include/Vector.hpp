#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>
#include <iostream>

class Vector
{
	public:
		float x, y, z;

		Vector() : x(0), y(0), z(1) {}
		Vector(float x, float y) : x(x), y(y), z(1) {}

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
		friend bool operator==(const Vector& v1, const Vector& v2);
};

#endif
