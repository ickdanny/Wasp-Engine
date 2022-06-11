#pragma once

#include <stdexcept>

namespace wasp::math {

	constexpr void throwIfZero(int i, const std::string& message = "int is zero!") {
		if (i == 0) {
			throw std::runtime_error{ message };
		}
	}
	constexpr void throwIfZero(float f, const std::string& message = "float is zero!") {
		if (f == 0.0f) {
			throw std::runtime_error{ message };
		}
	}

	template <typename T>
	constexpr T ceilingIntegerDivide(T x, T y) {
		static_assert(std::is_integral<T>::value, "integral type required!");
		return x / y + (x % y != 0);
	}
}