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
}