#pragma once

#include <stdexcept>

class FileError : public std::runtime_error {
public:
	FileError(char const* const message) noexcept
		: std::runtime_error{ message }{
	}
};