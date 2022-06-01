#include "Math/Vector2.h"

#include <stdexcept>

#include "Math/MathUtil.h"

namespace wasp::math {

	//conversion to point
	Vector2::operator Point2() const {
		return Point2{ x, y };
	}

	//conversion to string
	Vector2::operator std::string() const {
		return "<" + std::to_string(x) + ", " + std::to_string(y) + ">";
	}

    //mathematical assignment operators

    //vector addition and subtraction
    Vector2& Vector2::operator+=(const Vector2& right) {
        x += right.x;
        y += right.y;
        return *this;
    }
    Vector2& Vector2::operator-=(const Vector2& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }
    //scalar multiplication and division
    Vector2& Vector2::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2& Vector2::operator/=(float scalar) {
        throwIfZero(scalar, "vector divide by zero!"); 
        x /= scalar;
        y /= scalar;
        return *this;
    }

    //mathematical operators for Vector2 class

    //vector addition and subtraction
    Vector2 operator+(const Vector2& a, const Vector2& b) {
        return { a.x + b.x, a.y + b.y };
    }
    Vector2 operator-(const Vector2& a, const Vector2& b) {
        return { a.x - b.x, a.y - b.y };
    }
    //point addition and subtraction
    Point2 operator+(const Point2& a, const Vector2& b) {
        return { a.x + b.x, a.y + b.y };
    }
    Point2 operator-(const Point2& a, const Vector2& b) {
        return { a.x - b.x, a.y - b.y };
    }
    //scalar multiplication and division
    Vector2 operator*(const Vector2& vector, float scalar) {
        return { vector.x * scalar, vector.y * scalar };
    }
    Vector2 operator*(float scalar, const Vector2& vector){
        return { vector.x * scalar, vector.y * scalar };
    }
    Vector2 operator/(const Vector2& vector, float scalar){
        throwIfZero(scalar, "vector divide by zero!");
        return { vector.x / scalar, vector.y / scalar };
    }
    Vector2 operator/(float scalar, const Vector2& vector){
        throwIfZero(scalar, "vector divide by zero!");
        return { vector.x / scalar, vector.y / scalar };
    }
}