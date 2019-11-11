#pragma once
#include <math.h>
#include <iostream>

class Vector3
{
public:
	float elements[3];

	Vector3(float x, float y, float z);
	Vector3();

	float x();
	float y();
	float z();

	Vector3 operator+(); //Unary operators
	Vector3 operator-(); 

	float operator[](int index) const; //Read member
	float& operator[](int index); //To write to a member

	Vector3& operator +=(const Vector3& otherVector); //Vector with vector
	Vector3& operator*=(const Vector3 otherVector);
	Vector3& operator/=(const Vector3 otherVector);
	Vector3& operator-=(const Vector3 otherVector);

	Vector3& operator*=(const float scalar);
	Vector3& operator/=(const float scalar);

	Vector3 unit();
	float length();

};

std::ostream& operator<<(std::ostream& stream, const Vector3& vec);

float dot(const Vector3& first, const Vector3& second);

Vector3 operator+(const Vector3& first, const Vector3& second);
Vector3 operator-(const Vector3& first, const Vector3& second);
Vector3 operator*(const Vector3& first, const Vector3& second); //Note to self, component wise multiplication. Not dot.
Vector3 operator/(const Vector3& first, const Vector3& second);

Vector3 operator/(const Vector3& vector, float divisor);
Vector3 operator*(float divisor, const Vector3& vector);
Vector3 cross(Vector3& vec1, Vector3& vec2);