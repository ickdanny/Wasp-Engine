#pragma once

#include <memory>

#include "Scene.h"

namespace wasp::scene {

	//SystemCallEnumClass is the enum class used to identify different system chains
	//SceneNameEnumClass is the enum class used to identify different scenes
	template <typename SystemChainIDEnumClass, typename SceneNameEnumClass>
	class SceneStorage {
	private:
		//typedefs
		using Scene = Scene<SystemChainIDEnumClass, SceneNameEnumClass>;
		using ScenePointer = std::shared_ptr<Scene>;

		//fields
		std::vector<ScenePointer> scenePointers{};

	public:
		SceneStorage() = default;

		ScenePointer& getScenePointer(SceneNameEnumClass sceneName) {
			uint32_t index{ static_cast<uint32_t>(sceneName) };
			if (index < scenePointers.size()) {
				ScenePointer& scenePointer{ scenePointers[index] };
				if (scenePointer) {
					return scenePointer;
				}
			}
			throw std::runtime_error{ "unrecognized scene name in SceneStorage!" };
		}

		void makeScene(
			SceneNameEnumClass name,
			uint32_t initEntityCapacity,
			uint32_t initComponentCapacity,
			const std::vector<std::pair<SystemChainIDEnumClass, bool>>&
				systemChainTransparency,
			bool refresh
		) {
			uint32_t index{ static_cast<uint32_t>(name) };
			if (index >= scenePointers.size()) {
				scenePointers.resize(index + 1);
			}
			if (scenePointers[index]) {
				throw std::runtime_error{ "trying to add scene with same index!" };
			}
			scenePointers[index] = std::make_shared<Scene>(
				name,
				initEntityCapacity,
				initComponentCapacity,
				systemChainTransparency,
				refresh
			);
		}
	};
}