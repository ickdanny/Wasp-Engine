#pragma once

#ifdef _DEBUG
#include <iostream>
#endif

namespace wasp::debug {

	inline void log(const std::string& message) {
		#ifdef _DEBUG
		std::cerr << message << '\n';
		#endif
	}
}