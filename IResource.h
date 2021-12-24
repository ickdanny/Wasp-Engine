#pragma once

#include <string>

#include "IResourceStorage.h"

//members necessary for the resource tree
//unload, reload, remove, write done thru a pointer to the storage
namespace resource {
    class IResource {
    protected:
        IResource* parentPointer{}; //assume parents don't outlive children
        IResourceStorage* storagePointer{}; //assume resources don't outlive storages
        std::wstring id{};
        bool loaded{};

    public:
        IResource(const std::wstring& id, bool loaded)
            : parentPointer{ nullptr }
            , storagePointer{ nullptr }
            , id{ id }
            , loaded{ loaded } {
        }

        virtual ~IResource() {
            //not all resources have parents
            if (parentPointer) {
                parentPointer->removeChild(this);
            }
        }

        IResource* getParentPointer() {
            return parentPointer;
        }

        IResourceStorage* getStoragePointer() {
            return storagePointer;
        }

        void setParentPointer(IResource* parentPointer) {
            this->parentPointer = parentPointer;
        }

        void setStoragePointer(IResourceStorage* storagePointer) {
            this->storagePointer = storagePointer;
        }

        const std::wstring& getID() const {
            return id;
        }

        bool isLoaded() const {
            return loaded;
        }

    protected:
        virtual void removeChild(IResource* child) {}
    };
}