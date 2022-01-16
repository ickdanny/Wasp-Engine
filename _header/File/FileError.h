#pragma once

#include <stdexcept>

namespace wasp::file {

	class FileError : public std::runtime_error {
	public:
		explicit FileError(const std::string& what_arg)
			: std::runtime_error{ what_arg }{
		}

		explicit FileError(char const* const what_arg) noexcept
			: std::runtime_error{ what_arg }{
		}
	};
}