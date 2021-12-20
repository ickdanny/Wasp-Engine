#pragma once

#include <stdexcept>

class HResultError : public std::runtime_error {
public:
	HResultError(char const* const message) noexcept
		: std::runtime_error{ message }{
	}
};