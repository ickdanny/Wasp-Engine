#pragma once

#include <cstddef>

#include "Utility/Void.h"

namespace wasp::channel {

	template <typename T = utility::Void>
	struct Topic {
		const std::size_t index{};
	};
}