#include "Game/Systems/OverlaySystem.h"

namespace wasp::game::systems {

	void OverlaySystem::operator()(Scene& scene) {
        //get the group iterator for PlayerData
        static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
        auto groupPointer{
            getGroupPointer<PlayerData>(
                scene,
                groupPointerStorageTopic
            )
        };
        auto groupIterator{ groupPointer->groupIterator<PlayerData>() };

        //update all player states
        while (groupIterator.isValid()) {
            auto [playerData] = *groupIterator;
            updateOverlay(scene, playerData);
            ++groupIterator;
            break;      //we only update on the first player for now
        }
	}

    void OverlaySystem::updateOverlay(Scene& scene, const PlayerData& playerData) {
        SceneData& sceneData{ getSceneData(scene, playerData) };
        updateLives(scene, sceneData, playerData);
        updateBombs(scene, sceneData, playerData);
    }

    OverlaySystem::SceneData& OverlaySystem::getSceneData(
        Scene& scene, 
        const PlayerData& playerData
    ) {
        static const Topic<SceneData> sceneDataTopic{};
        auto& sceneDataChannel{ scene.getChannel(sceneDataTopic) };

        if (!sceneDataChannel.hasMessages()) {
            //create the overlay
            constexpr float initX{ 242.0f };
            constexpr float initY{ 28.0f };

            constexpr float xOffset{ 13.0f };
            constexpr float yOffset{ 27.0f };

            constexpr math::Vector2 offset{ xOffset, 0.0f };

            constexpr math::Point2 lifeInitPos{ initX, initY };
            constexpr math::Point2 bombInitPos{ initX, initY + yOffset };
            constexpr math::Point2 powerMeterPos{ 
                initX + (xOffset * (config::maxBombs - 1) / 2),
                bombInitPos.y + yOffset
            };

            auto& dataStorage{ scene.getDataStorage() };

            std::array<EntityHandle, config::maxLives> lifeHandles{};
            for (int i{ 0 }; i < config::maxLives; ++i) {
                lifeHandles[i] = dataStorage.addEntity(
                    makeIcon(
                        lifeInitPos,
                        offset,
                        i,
                        L"ui_life"
                    ).package()
                );
            }

            std::array<EntityHandle, config::maxBombs> bombHandles{};
            for (int i{ 0 }; i < config::maxBombs; ++i) {
                bombHandles[i] = dataStorage.addEntity(
                    makeIcon(
                        bombInitPos,
                        offset,
                        i,
                        L"ui_bomb"
                    ).package()
                );
            }

            EntityHandle lifeMeterHandle{
                dataStorage.addEntity(
                    makeIcon(
                        powerMeterPos,
                        {},
                        0,
                        playerData.power == config::maxPower 
                            ? L"ui_power_max" 
                            : L"ui_power"
                    ).package()
                )
            };
            //todo: power meter subimage

            SceneData sceneData{
                std::move(lifeHandles),
                std::move(bombHandles),
                lifeMeterHandle,
                -1,
                -1
            };
            sceneDataChannel.addMessage(sceneData);
        }
        return sceneDataChannel.getMessages()[0];
    }

    OverlaySystem::IconComponentTuple OverlaySystem::makeIcon(
        const math::Point2& initPos,
        const math::Vector2& offset,
        int index,
        const std::wstring& imageName
    ) const {
        math::Point2 position{ initPos + (offset * static_cast<float>(index)) };
        return EntityBuilder::makePosition(
            position,
            SpriteInstruction{
                bitmapStoragePointer->get(imageName)->d2dBitmap
            },
            DrawOrder{ config::foregroundDrawOrder + 1 }
        );
    }

    void OverlaySystem::updateLives(
        Scene& scene,
        SceneData& sceneData,
        const PlayerData& playerData
    ) {
        auto& dataStorage{ scene.getDataStorage() };
        auto& [lifeHandles, unused1, unused2, currentLifeIndex, unused3] = sceneData;

        int playerLifeIndex = playerData.lives - 1;
        if (playerLifeIndex >= -1) {
            if (currentLifeIndex > playerLifeIndex) {
                for (
                    int i{ std::min(currentLifeIndex, config::maxLives - 1) }; 
                    i > playerLifeIndex;
                    --i
                ) {
                    dataStorage.removeComponent<VisibleMarker>(
                        { lifeHandles[i] }
                    );
                }
            }
            else if (currentLifeIndex < playerLifeIndex) {
                for (
                    int i{ std::max(currentLifeIndex + 1, 0) }; 
                    i <= playerLifeIndex; 
                    ++i
                ) {
                    dataStorage.setComponent<VisibleMarker>({ lifeHandles[i], {} });
                }
            }
            currentLifeIndex = playerLifeIndex;
        }
    }

    void OverlaySystem::updateBombs(
        Scene& scene,
        SceneData& sceneData,
        const PlayerData& playerData
    ) {
        auto& dataStorage{ scene.getDataStorage() };
        auto& [unused1, bombHandles, unused2, unused3, currentBombIndex] = sceneData;

        int playerBombIndex = playerData.bombs - 1;
        if (playerBombIndex >= -1) {
            if (currentBombIndex > playerBombIndex) {
                for (
                    int i{ std::min(currentBombIndex, config::maxBombs - 1) };
                    i > playerBombIndex;
                    --i
                ) {
                    dataStorage.removeComponent<VisibleMarker>(
                        { bombHandles[i] }
                    );
                }
            }
            else if (currentBombIndex < playerBombIndex) {
                for (
                    int i{ std::max(currentBombIndex + 1, 0) };
                    i <= playerBombIndex;
                    ++i
                ) {
                    dataStorage.setComponent<VisibleMarker>({ bombHandles[i], {} });
                }
            }
            currentBombIndex = playerBombIndex;
        }
    }

    void OverlaySystem::updatePower(
        Scene& scene,
        SceneData& sceneData,
        const PlayerData& playerData
    ) {
        //todo: update power meter

        /*
                private void updatePower(AbstractECSInterface ecsInterface, AbstractPublishSubscribeBoard sliceBoard){
            AbstractDataStorage dataStorage = ecsInterface.getSliceData();
            SpriteInstruction spriteInstruction = dataStorage.getComponent(powerDisplay, spriteInstructionComponentType);

            int currentPower = playerData.getPower();
            if(currentPower == 0){
                if(dataStorage.containsComponent(powerDisplay, visibleMarker)){
                    sliceBoard.publishMessage(ECSUtil.makeRemoveComponentMessage(
                            new RemoveComponentOrder(powerDisplay, visibleMarker)
                    ));
                }
            }
            else{
                if(!dataStorage.containsComponent(powerDisplay, visibleMarker)){
                    sliceBoard.publishMessage(ECSUtil.makeAddComponentMessage(
                            new AddComponentOrder<>(powerDisplay, visibleMarker, null)
                    ));
                }
            }
            if (currentPower != MAX_POWER){
                if(!spriteInstruction.getImage().equals(POWER_IMAGE)){
                    spriteInstruction.setImage(POWER_IMAGE);
                }

                double ratio = ((double)currentPower)/MAX_POWER;
                int width = (int)(ratio * powerBarWidth);


                if(dataStorage.containsComponent(powerDisplay, spriteSubImageComponentType)) {
                    Rectangle subImage = dataStorage.getComponent(powerDisplay, spriteSubImageComponentType);
                    if (subImage.getWidth() != width) {
                        updatePowerBarWidth(sliceBoard, width);
                    }
                }
                else{
                    updatePowerBarWidth(sliceBoard, width);
                }
            }
            else{
                if(dataStorage.containsComponent(powerDisplay, spriteSubImageComponentType)){
                    sliceBoard.publishMessage(ECSUtil.makeRemoveComponentMessage(new RemoveComponentOrder(
                            powerDisplay,
                            spriteSubImageComponentType)
                    ));
                }

                if(!spriteInstruction.getImage().equals(POWER_MAX_IMAGE)){
                    spriteInstruction.setImage(POWER_MAX_IMAGE);
                }
            }
        }

        private void updatePowerBarWidth(AbstractPublishSubscribeBoard sliceBoard, int width){
            sliceBoard.publishMessage(ECSUtil.makeSetComponentMessage(new SetComponentOrder<>(
                    powerDisplay,
                    spriteSubImageComponentType,
                    //new Rectangle(powerBarWidth - width, 0, width, powerBarHeight)
                    new Rectangle(0, 0, width, powerBarHeight)
            )));
        }
        */
    }
}