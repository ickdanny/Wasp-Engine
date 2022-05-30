#pragma once

#include "Game/Scenes.h"

namespace wasp::game::systems {
	
	//Retrieves the group pointer from the channel for the given group pointer topic
	template <typename... Ts>
	ecs::component::Group* getGroupPointer(
		Scene& scene, 
		channel::Topic<ecs::component::Group*> groupPointerStorageTopic
	) {
		ecs::DataStorage& dataStorage{ scene.getDataStorage() };

		auto& channel{ scene.getChannel(groupPointerStorageTopic) };
		ecs::component::Group* groupPointer;	//not initialized!
		if (channel.isEmpty()) {
			groupPointer = dataStorage.getGroupPointer<Ts...>();
			channel.addMessage(groupPointer);
		}
		else {
			groupPointer = channel.getMessages()[0];
		}
		return groupPointer;
	}
}