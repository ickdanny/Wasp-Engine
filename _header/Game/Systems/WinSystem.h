#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class WinSystem {
	private:
		//fields
		channel::ChannelSet* globalChannelSetPointer{};

	public:
		WinSystem(channel::ChannelSet* globalChannelSetPointer)
			: globalChannelSetPointer{ globalChannelSetPointer } {
		}

		void operator()(Scene& scene);

	private:
		//helper functions
		void enterCreditsScreen();
	};
}