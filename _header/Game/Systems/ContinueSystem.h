#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class ContinueSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;
		using SceneData = std::tuple<EntityHandle, int>;

		//fields
		channel::ChannelSet* globalChannelSetPointer{};

	public:
		ContinueSystem(channel::ChannelSet* globalChannelSetPointer)
			: globalChannelSetPointer{ globalChannelSetPointer } {
		}

		void operator()(Scene& scene);

	private:
		void handleContinue(Scene& scene, const EntityHandle& playerHandle);
		void enterContinueMenu(Scene& scene, const EntityHandle& playerHandle);
	};
}