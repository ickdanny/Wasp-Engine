#include "Math/Vector2.h"

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

    //mathematical operators for Vector2 class
    Vector2 operator+(const Vector2& a, const Vector2& b) {
        return { a.x + b.x, a.y + b.y };
    }
    Vector2 operator-(const Vector2& a, const Vector2& b) {
        return { a.x - b.x, a.y - b.y };
    }
    Point2 operator+(const Point2& a, const Vector2& b) {
        return { a.x + b.x, a.y + b.y };
    }
    Point2 operator-(const Point2& a, const Vector2& b) {
        return { a.x - b.x, a.y - b.y };
    }
}