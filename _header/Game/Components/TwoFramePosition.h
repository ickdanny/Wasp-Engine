#pragma once

#include "Math/Point2.h"
#include "Math/Vector2.h"
#include "Utility/TwoFrame.h"

namespace wasp::game::components {

	class TwoFramePosition : public utility::TwoFrame<math::Point2> {
	private:
		//typedefs
		using TwoFrame = utility::TwoFrame<math::Point2>;
		using Point2 = math::Point2;
		using Vector2 = math::Vector2;

	public:
		using utility::TwoFrame<math::Point2>::TwoFrame;

		//Explicitly define copy and move constructors taking Point2
		TwoFramePosition(const math::Point2& toCopy)
			: TwoFrame(toCopy) {
		}
		TwoFramePosition(math::Point2&& toMove)
			: TwoFrame(toMove) {
		}

		//mathematical assignment operators
		TwoFramePosition& operator+=(const Vector2& right) {
			x += right.x;
			y += right.y;
			return *this;
		}
		TwoFramePosition& operator-=(const Vector2& right) {
			x -= right.x;
			y -= right.y;
			return *this;
		}
	};

	/*
	class TwoFramePosition : public math::Point2 {
	private:
		//typedefs
		using Point2 = math::Point2;
		using Vector2 = math::Vector2;

		//fields
		Point2 past{};

	public:
		//constructors
		TwoFramePosition() = default;

		TwoFramePosition(float x, float y)
			: Point2{ x, y }
			, past{ x, y } {
		}

		TwoFramePosition(const Point2& pos)
			: Point2 { pos }
			, past { pos } {
		}

		//puts the current position into past storage
		void step() {
			past = *this;
		}

		//retrieves the stored previous position
		const Point2& getPast() {
			return past;
		}

		//mathematical assignment operators
		TwoFramePosition& operator+=(const Vector2& right) {
			x += right.x;
			y += right.y;
		}
		TwoFramePosition& operator-=(const Vector2& right) {
			x -= right.x;
			y -= right.y;
		}
	};
	*/
}