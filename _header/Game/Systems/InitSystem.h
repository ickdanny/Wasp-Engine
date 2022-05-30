#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"
#include "Game/Systems/EntityBuilder.h"

namespace wasp::game::systems {

	class InitSystem {
	private:
		resources::BitmapStorage* bitmapStoragePointer{};

	public:
		InitSystem(resources::BitmapStorage* bitmapStoragePointer)
			: bitmapStoragePointer{ bitmapStoragePointer } {
		}

		void operator()(Scene& scene) const;

		//helper functions
	private:
		void initScene(Scene& scene) const;
		void initMainMenu(Scene& scene) const;

		void addBackground(ecs::DataStorage& dataStorage, std::wstring name) const;

		ComponentTuple<
			Position,
			VisibleMarker,
			ButtonData,
			MenuCommandSelect,
			ButtonAction,
			SpriteInstruction,
			DrawOrder
		>
		makeButton(
			const ButtonData& buttonData,
			MenuCommandSelect selectCommand,
			const ButtonAction& buttonAction = {},
			DrawOrder drawOrder = { 0 },
			bool selected = false
		) const;

		//attaches buttons from top down
		void attachButtonsVertical(
			ecs::DataStorage& dataStorage,
			const std::vector<ecs::entity::EntityHandle>& entityHandles
		) const;

		//attaches buttons from left to right
		void attachButtonsHorizontal(
			ecs::DataStorage& dataStorage,
			const std::vector<ecs::entity::EntityHandle>& entityHandles
		) const;
	};
}