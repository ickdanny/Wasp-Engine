#pragma once

namespace wasp::utility {
	template<typename T>
	inline T ceilingIntegerDivide(T x, T y) {
		return x / y + (x % y != 0);
	}
}