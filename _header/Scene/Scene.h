#pragma once

#include "ECS/DataStorage.h"
#include "Channel/ChannelSet.h"

namespace wasp::scene {

	//SystemCallEnumClass is the enum class used to identify different system chains
	//SceneNameEnumClass is the enum class used to identify different scenes
	template <typename SystemChainIDEnumClass, typename SceneNameEnumClass>
	class Scene {
	private:
		SceneNameEnumClass name{};
		uint32_t initEntityCapacity{};
		uint32_t initComponentCapacity{};
		ecs::DataStorage dataStorage;	//not initialized!
		channel::ChannelSet channelSet{};
		std::vector<bool> systemChainTransparency{};
		bool refresh{};

	public:
		Scene(
			SceneNameEnumClass name,
			uint32_t initEntityCapacity, 
			uint32_t initComponentCapacity,
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

		const auto& getDataStorage() const {
			return dataStorage;
		}

		template <typename T>
		bool hasChannel(const channel::Topic<T>& topic) const {
			return channelSet.hasChannel(topic);
		}

		template <typename T>
		channel::Channel<T>& getChannel(const channel::Topic<T>& topic) {
			return channelSet.getChannel(topic);
		}

		template <typename T>
		const channel::Channel<T>& getChannel(const channel::Topic<T>& topic) const {
			return channelSet.getChannel(topic);
		}

		bool isTransparent(SystemChainIDEnumClass systemChainID) const {
			uint32_t index{ static_cast<uint32_t>(systemChainID) };
			if (index >= systemChainTransparency.size()) {
				throw std::runtime_error{ "system chain index out of bounds!" };
			}
			return systemChainTransparency[index];
		}

		bool needsRefresh() const {
			return refresh;
		}

		void refreshScene() {
			dataStorage.recreate();
			channelSet.clear();
		}

	private:
		//helper methods
		void setSystemChainTransparency(
			SystemChainIDEnumClass systemChainID,
			bool transparency
		) {
			uint32_t index{ static_cast<uint32_t>(systemChainID) };
			if (index >= systemChainTransparency.size()) {
				systemChainTransparency.resize(index + 1, false);
			}
			systemChainTransparency[index] = transparency;
		}
	};
}