#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class LoadSystem {
	private:
		//fields
		channel::ChannelSet* globalChannelSetPointer{};

	public:
		LoadSystem(channel::ChannelSet* globalChannelSetPointer);
		void operator()(Scene& scene);
	};
}