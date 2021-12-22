#pragma once

#include <string>

namespace resource {
    template<typename T>
    class IResourceManager {
    public:
        virtual ~IResourceManager() = default;
        virtual Resource<T> getResource(const std::string& id) = 0;
        virtual void loadResource(Resource<T> resource) = 0;
        virtual void reloadResource(const std::string& id) = 0;
        virtual void unloadResource(const std::string& id) = 0;
        virtual void removeResource(const std::string& id) = 0;
        virtual void cleanUp() = 0;
    };
}