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

namespace resource {

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
                std::get<1>(*found)->unloadData();
            }
        }

        void reload(const std::wstring& id) override {
            if (resourceLoaderPointer) {
                auto found{ resourceMap.find(id) };
                if (found != resourceMap.end()) {
                    const ResourceOriginVariant origin{ 
                        std::get<1>(*found)->getOrigin()
                    };
                    switch (origin.index()) {
                        case 0: {
                            FileOrigin const* fileTest{
                                std::get_if<FileOrigin>(&origin)
                            };
                            if (fileTest) {
                                resourceLoaderPointer->loadFile(*fileTest);
                            }
                            break;
                        }
                        case 1: {
                            ManifestOrigin const* manifestTest{
                                std::get_if<ManifestOrigin>(&origin)
                            };
                            if (manifestTest) {
                                resourceLoaderPointer->loadManifestEntry(
                                    *manifestTest
                                );
                            }
                            break;
                        }
                    }
                }
            }
            else {
                throw std::runtime_error{ "Error trying to reload without loader" };
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