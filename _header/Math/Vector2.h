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
        Vector2& operator+=(const Vector2& right);
        Vector2& operator-=(const Vector2& right);
        
	};

    //mathematical operators for Vector2 class
    Vector2 operator+(const Vector2& a, const Vector2& b);
    Vector2 operator-(const Vector2& a, const Vector2& b);
    Point2 operator+(const Point2& a, const Vector2& b);
    Point2 operator-(const Point2& a, const Vector2& b);

    //not implementing equivalence operators due to floating point imprecision
}