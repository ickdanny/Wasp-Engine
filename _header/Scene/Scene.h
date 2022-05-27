#pragma once

#include "ECS/DataStorage.h"
#include "Channel/Channel.h"
#include "Channel/Topic.h"

namespace wasp::scene {

	//SystemCallEnumClass is the enum class used to identify different system chains
	//SceneNameEnumClass is the enum class used to identify different scenes
	template <typename SystemChainIDEnumClass, typename SceneNameEnumClass>
	class Scene {
	private:
		SceneNameEnumClass name{};
		std::size_t initEntityCapacity{};
		std::size_t initComponentCapacity{};
		ecs::DataStorage dataStorage;	//not initialized!
		std::vector<std::unique_ptr<int>> channels{};
		std::vector<bool> systemChainTransparency{};
		bool refresh{};

	public:
		Scene(
			SceneNameEnumClass name,
			std::size_t initEntityCapacity, 
			std::size_t initComponentCapacity,
			const std::vector<std::pair<SystemChainIDEnumClass, bool>>&
				systemChainTransparency,
			bool refresh
		)
			: name{ name }
			, initEntityCapacity { initEntityCapacity }
			, initComponentCapacity{ initComponentCapacity }
			, dataStorage{ initEntityCapacity, initComponentCapacity }
			, refresh{ refresh }
		{
			for (const auto [systemChainID, transparency] : systemChainTransparency) {
				setSystemChainTransparency(systemChainID, transparency);
			}
		}

		auto getName() const {
			return name;
		}

		auto& getDataStorage() {
			return dataStorage;
		}

		template <typename T>
		channel::Channel<T>& getChannel(const channel::Topic<T> topic) {
			//resize the pointer vector if necessary
			if (topic.index >= channels.size()) {
				channels.resize(topic.index + 1);
			}
			auto& channelPointer{ channels[topic.index] };
			//if stored pointer is not nullptr, return the result of dereference
			if (channelPointer) {
				return static_cast<channel::Channel<T>>(*channelPointer);
			}
			//otherwise, emplace a new channel and return that
			else {
				channels.emplace(
					channels.begin() + topic.index,
					channel::Channel<T>{}
				);
				return static_cast<channel::Channel<T>>(*channels[topic.index]);
			}
		}

		bool isTransparent(SystemChainIDEnumClass systemChainID) const {
			int index{ static_cast<int>(systemChainID) };
			if (index >= systemChainTransparency.size()) {
				throw std::runtime_error{ "system chain index out of bounds!" };
			}
			return systemChainTransparency[index];
		}

		bool needsRefresh() const {
			return refresh;
		}

		void refreshScene() {
			dataStorage = { initEntityCapacity, initComponentCapacity };
			channels.clear();
		}

	private:
		//helper methods

		void setSystemChainTransparency(
			SystemChainIDEnumClass systemChainID,
			bool transparency
		) {
			std::size_t index{ static_cast<std::size_t>(systemChainID) };
			if (index >= systemChainTransparency.size()) {
				systemChainTransparency.resize(index + 1, false);
			}
			systemChainTransparency[index] = transparency;
		}
	};
}