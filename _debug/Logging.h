#pragma once

#ifdef _DEBUG
#include <iostream>
#endif

namespace wasp::debug {

	void log(const std::string& message) {
		#ifdef _DEBUG
		std::cerr << message;
		#endif
	}
}