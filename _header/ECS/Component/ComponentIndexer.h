#pragma once

#include <cstddef>
#include <cstdint>

namespace wasp::ecs::component {
    //thanks to a user named DragonSlayer0531

    class ComponentIndexer{
    private:
        static uint32_t indexCounter;

    public:
        template <typename T>
        static uint32_t getIndex() {
            static uint32_t typeIndex = indexCounter++;
            return typeIndex;
        }
    };
}