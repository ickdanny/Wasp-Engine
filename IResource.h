#pragma once

#include <string>

#include "IResourceStorage.h"

//members necessary for the resource tree
//unload, reload, remove, write done thru a pointer to the storage
namespace resource {
    class IResource {
    protected:
        std::weak_ptr<IResource> parent{};
        IResourceStorage* storagePointer; //assume resources don't outlive storages
    public:
        virtual ~IResource() {
            if (!parent.expired()) {
                parent.lock()->removeChild(this);
            }
        }
    protected:
        virtual void removeChild(IResource* child) {}
    };
}