#pragma once

#include "Point2.h"

namespace wasp::math {
	struct Vector2 {
		float x{};
		float y{};

        //unary negative operator
        Vector2 operator-() const {
            return { -x, -y };
        }

        //conversion to point
        explicit operator Point2() const;

        //conversion to string
        explicit operator std::string() const;

        //mathematical assignment operators

        //vector addition and subtraction
        Vector2& operator+=(const Vector2& right);
        Vector2& operator-=(const Vector2& right);
        //scalar multiplication and division
        Vector2& operator*=(float scalar);
        Vector2& operator/=(float scalar);
	};

    //mathematical operators for Vector2 class

    //vector addition and subtraction
    Vector2 operator+(const Vector2& a, const Vector2& b);
    Vector2 operator-(const Vector2& a, const Vector2& b);
    //point addition and subtraction
    Point2 operator+(const Point2& a, const Vector2& b);
    Point2 operator-(const Point2& a, const Vector2& b);
    //scalar multiplication and division
    Vector2 operator*(const Vector2& vector, float scalar);
    Vector2 operator*(float scalar, const Vector2& vector);
    Vector2 operator/(const Vector2& vector, float scalar);
    Vector2 operator/(float scalar, const Vector2& vector);

    //not implementing equivalence operators due to floating point imprecision
}