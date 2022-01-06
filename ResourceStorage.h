#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "Resource.h"
#include "IResourceStorage.h"
#include "ResourceLoader.h"
#include "UnsupportedOperationError.h"

namespace wasp::resource {

    template <typename T>
    class ResourceStorage : public IResourceStorage{
        using ResourceMap = 
            std::unordered_map<std::wstring, std::shared_ptr<Resource<T>>>;

    protected:
        ResourceMap resourceMap{};
        ResourceLoader* resourceLoaderPointer{}; //for reloading? is this dumb?

    public:
        ResourceStorage() = default;
        virtual ~ResourceStorage() = default;

        void unload(const std::wstring& id) override {
            auto found{ resourceMap.find(id) };
            if (found != resourceMap.end()) {
                Resource<T>& resource{
                    *(std::get<1>(*found))
                };

                if (resource.isLoaded()) {
                    resource.unloadData();
                }
            }
        }

        void remove(const std::wstring& id) override {
            //calls parent.removeChild in destructor
            resourceMap.erase(id);
        }

        virtual std::shared_ptr<T> get(const std::wstring& id) {
            auto found{ resourceMap.find(id) };
            if (found != resourceMap.end()) {
                return std::get<1>(*found)->getDataPointerCopy();
            }
            else {
                return nullptr;
            }
        }

        void setLoader(ResourceLoader* resourceLoaderPointer) {
            this->resourceLoaderPointer = resourceLoaderPointer;
        }

        void forEach(
            std::function<void(std::shared_ptr<Resource<T>>)> callBackFunction
        ) {
            std::for_each(
                resourceMap.begin(), 
                resourceMap.end(), 
                [&](auto pairElement) {
                    callBackFunction(std::get<1>(pairElement));
                }
            );
        }
    };
}