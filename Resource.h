#pragma once

#include <string>
#include <memory>

#include "IResource.h"
#include "ResourceOriginVariant.h"

namespace resource {
    template<typename T>
    class Resource : IResource{
    protected:
        std::wstring id{};
        ResourceOriginVariant origin{};
        bool loaded{};
        std::shared_ptr<T> dataPointer{};

    public:
        Resource(
            const std::wstring& id,
            const IResourceOrigin& origin,
            const IResourceType& type
        )
            : id{id}, origin{origin}, type{type}, loaded{ false }{
        }

        Resource(
            const std::wstring& id, 
            const IResourceOrigin& origin, 
            std::shared_ptr<T> dataPointer, 
            const IResourceType& type
        )
            : id{id}
            , origin{origin}
            , dataPointer{ dataPointer }
            , type{type}
            , loaded{ true }{
        }

        const std::wstring& getId() const {
            return id;
        }
        const ResourceOriginVariant& getOrigin() const {
            return origin;
        }
        bool isLoaded() const {
            return loaded;
        }
        std::shared_ptr<T> getDataPointerCopy() {
            return dataPointer;
        }
        const std::shared_ptr<const T> getDataPointerCopy() const {
            return dataPointer;
        }
        void setData(std::shared_ptr<T> dataPointer) {
            this->dataPointer = dataPointer;
            loaded = dataPointer;
        }
        void unloadData() {
            dataPointer = nullptr;
            loaded = false;
        }
    };
}