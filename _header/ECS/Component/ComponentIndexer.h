#pragma once

namespace wasp::ecs::component {
    //thanks to a user named DragonSlayer0531

    class ComponentIndexer{
    private:
        static int indexCounter;

    public:
        template <typename T>
        static int getIndex() {
            static int typeIndex = indexCounter++;
            return typeIndex;
        }

        static int getNumComponents() {
            return indexCounter;
        }
        static int getMaxIndex() {
            indexCounter - 1;
        }
    };

    int ComponentIndexer::indexCounter{ 0 };
}