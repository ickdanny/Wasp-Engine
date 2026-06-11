#pragma once

#include <cstddef>
#include <cstdint>

#include "Utility/Void.h"

namespace wasp::channel {

	struct TopicBase {
	protected:
		static uint32_t indexer;

	public:
		const uint32_t index{};

		TopicBase()
			: index{ indexer++ } {
		}
	};

	template <typename T = utility::Void>
	struct Topic : public TopicBase{
		using Void = utility::Void;
	};
}