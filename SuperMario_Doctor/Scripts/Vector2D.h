#pragma once
#include <iostream>

class Vector2D
{
private:
	float x;
	float y;

	Vector2D& add(const Vector2D& vec);
	Vector2D& subtract(const Vector2D& vec);
	Vector2D& multiply(const Vector2D& vec);
	Vector2D& divide(const Vector2D& vec);

public:


	Vector2D(float x = 0, float y = 0);

	const float getX() const { return x; };
	const float getY() const { return y; };

	void setX(const float value) { this->x = value; };
	void setY(const float value) { this->y = value; };

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	 Vector2D& operator+=(const Vector2D& vec);
	 Vector2D& operator-=(const Vector2D& vec);
	 Vector2D& operator*=(const Vector2D& vec);
	 Vector2D& operator/=(const Vector2D& vec);
	 Vector2D& operator*(const int& i);
	 Vector2D& Zero();

	 friend std::ostream& operator<<(std::ostream os, const Vector2D& vec);

};