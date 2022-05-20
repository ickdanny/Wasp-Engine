#pragma once

namespace wasp::ecs::component {
    //thanks to a user named DragonSlayer0531

    class TypeIndexRetriever{
    private:
        static int indexCounter;

    public:
        template <typename T>
        static int retrieveIndex() {
            static int typeIndex = indexCounter++;
            return typeIndex;
        }

        static int maxIndex() {
            return indexCounter - 1;
        }
    };

    int TypeIndexRetriever::indexCounter{ 0 };
}