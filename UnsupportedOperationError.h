#pragma once

#include <stdexcept>

namespace wasp::exception {
	//translation from java
	class UnsupportedOperationError : public std::runtime_error {
	public:
		UnsupportedOperationError(char const* const message) noexcept
			: std::runtime_error{ message }{
		}
	};
}