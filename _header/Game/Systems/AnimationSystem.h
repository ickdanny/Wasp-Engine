#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"

namespace wasp::game::systems {

    class AnimationSystem {

    private:
        //typedefs
        using EntityID = ecs::entity::EntityID;
        using EntityHandle = ecs::entity::EntityHandle;
        using DataStorage = ecs::DataStorage;
        using Animation = components::Animation;

        //fields
        resources::BitmapStorage* bitmapStoragePointer{};

    public:
        AnimationSystem(resources::BitmapStorage* bitmapStoragePointer);
        void operator()(Scene& scene);

    private:
        //helper functions

        //returns true if we need to remove this entity's animation list
        bool handleAnimation(
            DataStorage& dataStorage, 
            AnimationList& animationList,
            const EntityHandle& entityHandle
        );
        //returns true if animation changed, false otherwise.
        bool handleTurning(
            DataStorage& dataStorage,
            AnimationList& animationList,
            const EntityHandle& entityHandle
        );
        bool tryToTurnLeft(AnimationList& animationList);
        bool tryToTurnRight(AnimationList& animationList);
        bool tryToTurnCenter(AnimationList& animationList);

        //returns true if end of animation
        bool stepAnimation(AnimationList& animationList);
    };
}