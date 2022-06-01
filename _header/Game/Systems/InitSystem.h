#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"
#include "Game/Systems/EntityBuilder.h"

namespace wasp::game::systems {

	class InitSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;
		using BasicButtonComponentTuple = ComponentTuple<
			Position,
			VisibleMarker,
			ButtonData,
			MenuCommandSelect,
			SpriteInstruction,
			DrawOrder
		>;

		//fields
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

		BasicButtonComponentTuple makeButton(
			const math::Point2& initPos,
			const math::Vector2& offset,
			const math::Vector2& selOffset,
			int index,
			const std::wstring& name,
			MenuCommandSelect selectCommand,
			DrawOrder drawOrder = { 0 },
			bool selected = false
		) const;

		//attaches buttons from top down
		void attachButtonsVertical(
			ecs::DataStorage& dataStorage,
			const std::vector<EntityHandle>& entityHandles
		) const;

		//attaches buttons from left to right
		void attachButtonsHorizontal(
			ecs::DataStorage& dataStorage,
			const std::vector<EntityHandle>& entityHandles
		) const;

		void setInitSelectedElement(Scene& scene, const EntityHandle& entityHandle)
			const;
	};
}