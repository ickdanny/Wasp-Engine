#pragma once

#include <stdexcept>

namespace wasp::sound::midi {
	class MidiError : public std::runtime_error {
	public:
		explicit MidiError(const std::string& what_arg)
			: std::runtime_error{ what_arg }{
		}

		explicit MidiError(const char* what_arg)
			: std::runtime_error{ what_arg }{
		}
	};
}