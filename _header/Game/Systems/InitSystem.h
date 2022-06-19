#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"
#include "Game/Systems/EntityBuilder.h"
#include "Game/Systems/SpawnPrograms/SpawnPrograms.h"

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
		channel::ChannelSet* globalChannelSetPointer{};
		resources::BitmapStorage* bitmapStoragePointer{};
		SpawnPrograms* spawnProgramsPointer{};

	public:
		InitSystem(
			channel::ChannelSet* globalChannelSetPointer, 
			resources::BitmapStorage* bitmapStoragePointer,
			SpawnPrograms* spawnProgramsPointer
		)
			: globalChannelSetPointer{ globalChannelSetPointer }
			, bitmapStoragePointer { bitmapStoragePointer } 
			, spawnProgramsPointer{ spawnProgramsPointer } {
		}

		void operator()(Scene& scene) const;

		//helper functions
	private:
		void initScene(Scene& scene) const;
		void initMainMenu(Scene& scene) const;
		void initDifficultyMenu(Scene& scene) const;
		void initShotMenu(Scene& scene) const;
		void initStageMenu(Scene& scene) const;
		void initOptionsMenu(Scene& scene) const;
		void initContinueMenu(Scene& scene) const;

		void initGame(Scene& scene) const;
		void initPauseMenu(Scene& scene) const;

		void addBackground(
			ecs::DataStorage& dataStorage, 
			const std::wstring& name,
			int relativeDrawOrder = 0,
			const math::Point2& position = 
				{ config::graphicsWidth / 2.0f, config::graphicsHeight / 2.0f }
		) const;
		void addForeground(
			ecs::DataStorage& dataStorage, 
			const std::wstring& name,
			int relativeDrawOrder = 0,
			const math::Point2& position =
				{ config::graphicsWidth / 2.0f, config::graphicsHeight / 2.0f }
		) const;

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

		BasicButtonComponentTuple makeButton(
			const math::Point2& unselPos,
			const math::Vector2& selOffset,
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