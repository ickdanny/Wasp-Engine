#pragma once

#include <stdexcept>

namespace wasp::exception {
	//translation from java
	class UnsupportedOperationError : public std::runtime_error {
	public:
		explicit UnsupportedOperationError(const std::string& what_arg)
			: std::runtime_error{ what_arg }{
		}
		explicit UnsupportedOperationError(char const* const what_arg) noexcept
			: std::runtime_error{ what_arg }{
		}
	};
}