#include "Vector3.h"

Vector3::Vector3(double x, double y, double z)
{
	elements[0] = x;
	elements[1] = y;
	elements[2] = z;
}

Vector3::Vector3()
{
	elements[0] = 0;
	elements[1] = 0;
	elements[2] = 0;
}

double Vector3::x()
{
	return elements[0];
}

double Vector3::y()
{
	return elements[1];
}

double Vector3::z()
{
	return elements[2];
}

Vector3 Vector3::operator+()
{
	return *this;
}

Vector3 Vector3::operator-()
{
	return Vector3(-elements[0], -elements[1], -elements[2]);
}

double Vector3::operator[](int index) const
{
	return elements[index];
}

double& Vector3::operator[](int index)
{
	return elements[index];
}

Vector3& Vector3::operator+=(const Vector3& otherVector)
{
	elements[0] += otherVector.elements[0];
	elements[1] += otherVector.elements[1];
	elements[2] += otherVector.elements[2];

	return *this;
}

Vector3& Vector3::operator*=(const Vector3 otherVector)
{
	elements[0] *= otherVector.elements[0];
	elements[1] *= otherVector.elements[1];
	elements[2] *= otherVector.elements[2];

	return *this;
}

Vector3& Vector3::operator/=(const Vector3 otherVector)
{
	elements[0] /= otherVector.elements[0];
	elements[1] /= otherVector.elements[1];
	elements[2] /= otherVector.elements[2];

	return *this;
}

Vector3& Vector3::operator-=(const Vector3 otherVector)
{
	elements[0] -= otherVector.elements[0];
	elements[1] -= otherVector.elements[1];
	elements[2] -= otherVector.elements[2];

	return *this;
}

Vector3& Vector3::operator*=(const double scalar)
{
	elements[0] *= scalar;
	elements[1] *= scalar;
	elements[2] *= scalar;

	return *this;
}

Vector3& Vector3::operator/=(const double scalar)
{
	elements[0] /= scalar;
	elements[1] /= scalar;
	elements[2] /= scalar;

	return *this;
}

Vector3 Vector3::unit()
{
	return Vector3(elements[0], elements[1], elements[2]) / length();
}

double Vector3::length()
{
	return sqrt(x() * x() + y() * y() + z() * z());
}

std::ostream& operator<<(std::ostream& stream, const Vector3& vec)
{
	stream << vec.elements[0] << " " << vec.elements[1] << " " << vec.elements[2];

	return stream;
}

Vector3 operator+(const Vector3& first, const Vector3& second)
{
	return Vector3(first.elements[0] + second.elements[0], first.elements[1] + second.elements[1], first.elements[2] + second.elements[2]);
}

Vector3 operator-(const Vector3& first, const Vector3& second)
{
	return Vector3(first.elements[0] - second.elements[0], first.elements[1] - second.elements[1], first.elements[2] - second.elements[2]);
}

Vector3 operator*(const Vector3& first, const Vector3& second)
{
	return Vector3(first.elements[0] * second.elements[0], first.elements[1] * second.elements[1], first.elements[2] * second.elements[2]);
}

Vector3 operator/(const Vector3& second, const Vector3& first)
{
	return Vector3(first.elements[0] / second.elements[0], first.elements[1] / second.elements[1], first.elements[2] / second.elements[2]);
}

Vector3 operator/(const Vector3& vector, double divisor)
{
	return Vector3(vector.elements[0] / divisor, vector.elements[1] / divisor, vector.elements[2] / divisor);
}

Vector3 operator*(double mult, const Vector3& vector)
{
	return Vector3(vector.elements[0] * mult, vector.elements[1] * mult, vector.elements[2] * mult);
}

Vector3 cross(Vector3& vec1, Vector3& vec2)
{
	return Vector3(vec1.elements[1] * vec2.elements[2] - vec1.elements[2] * vec2.elements[1], vec1.elements[2] * vec2.elements[0] - vec1.elements[0] * vec2.elements[2], vec1.elements[0] * vec2.elements[1] - vec1.elements[1] * vec2.elements[0]);

}

double dot(const Vector3& first, const Vector3& second)
{
	return first.elements[0] * second.elements[0] + first.elements[1] * second.elements[1] + first.elements[2] * second.elements[2];
}


