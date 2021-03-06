#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PauseSystem {
	private:
		//fields
		channel::ChannelSet* globalChannelSetPointer{};

	public:
		PauseSystem(channel::ChannelSet* globalChannelSetPointer)
			: globalChannelSetPointer{ globalChannelSetPointer } {
		}

		void operator()(Scene& scene);

	private:
		void enterPauseMenu(Scene& scene);
	};
}