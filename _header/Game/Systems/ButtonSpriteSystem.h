#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"

namespace wasp::game::systems {

    class ButtonSpriteSystem {

    private:
        //typedefs
        using EntityHandle = ecs::entity::EntityHandle;
        using DataStorage = ecs::DataStorage;

        //fields
        resources::BitmapStorage* bitmapStoragePointer{};

    public:
        ButtonSpriteSystem(resources::BitmapStorage* bitmapStoragePointer);

        void operator()(Scene& scene);

    private:
        //helper functions
        void selectButton(DataStorage& dataStorage, const EntityHandle& buttonHandle);
        void unselectButton(DataStorage& dataStorage, const EntityHandle& buttonHandle);
        void changeSpriteAndPosition(
            DataStorage& dataStorage,
            const EntityHandle& buttonHandle,
            const std::wstring& spriteName,
            const math::Point2& position
        );
    };
}