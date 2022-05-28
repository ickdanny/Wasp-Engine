#include "Math/PolarVector.h"

#include <cmath>

namespace wasp::math {

	PolarVector::PolarVector(float magnitude, Angle angle)
		: magnitude{ magnitude }
		, angle{ angle } 
	{
		updateVector2Representation();
	}

	void PolarVector::setMagnitude(float magnitude) {
		this->magnitude = magnitude;
		updateVector2Representation();
	}
	void PolarVector::setAngle(Angle angle) {
		this->angle = angle;
		updateVector2Representation();
	}

	PolarVector::operator Vector2() const {
		return vector2Representation;
	}

	void PolarVector::updateVector2Representation() {
		float radians{ angle.getAngleRadians() };
		vector2Representation.x = magnitude * std::cos(radians);
		vector2Representation.y = -1 * magnitude * std::sin(radians);
	}
}