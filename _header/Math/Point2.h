#pragma once

#include <string>

namespace wasp::math {
	struct Point2 {
		float x{};
		float y{};

		//conversion to string
		explicit operator std::string() const;
	};

	//not implementing any operators due to floating point imprecision
}