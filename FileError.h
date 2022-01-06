#pragma once

#include <stdexcept>

namespace wasp::file {

	class FileError : public std::runtime_error {
	public:
		FileError(char const* const message) noexcept
			: std::runtime_error{ message }{
		}
	};
}