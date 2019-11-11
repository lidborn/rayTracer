#pragma once
#include <math.h>
#include <iostream>

class Vector3
{
public:
	double elements[3];

	Vector3(double x, double y, double z);
	Vector3();

	double x();
	double y();
	double z();

	Vector3 operator+(); //Unary operators
	Vector3 operator-(); 

	double operator[](int index) const; //Read member
	double& operator[](int index); //To write to a member

	Vector3& operator +=(const Vector3& otherVector); //Vector with vector
	Vector3& operator*=(const Vector3 otherVector);
	Vector3& operator/=(const Vector3 otherVector);
	Vector3& operator-=(const Vector3 otherVector);

	Vector3& operator*=(const double scalar);
	Vector3& operator/=(const double scalar);

	Vector3 unit();
	double length();

};

std::ostream& operator<<(std::ostream& stream, const Vector3& vec);

double dot(const Vector3& first, const Vector3& second);

Vector3 operator+(const Vector3& first, const Vector3& second);
Vector3 operator-(const Vector3& first, const Vector3& second);
Vector3 operator*(const Vector3& first, const Vector3& second); //Note to self, component wise multiplication. Not dot.
Vector3 operator/(const Vector3& first, const Vector3& second);

Vector3 operator/(const Vector3& vector, double divisor);
Vector3 operator*(double divisor, const Vector3& vector);
Vector3 cross(Vector3& vec1, Vector3& vec2);