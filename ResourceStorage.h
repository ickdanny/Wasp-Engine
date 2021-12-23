#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Resource.h"
#include "IResourceStorage.h"
#include "UnsupportedOperationError.h"

namespace resource {

    template <typename T>
    class ResourceStorage : public IResourceStorage{
        using ResourceMap = 
            std::unordered_map<std::string&, std::shared_ptr<Resource<T>>>;

    private:
        ResourceMap resourceMap{};

    public:
        ResourceStorage() = default;
        virtual ~ResourceStorage() = default;

        void unload(const std::wstring& id) override {

        }

        virtual std::shared_ptr<T> get(const std::wstring& id) {
            auto found{ resourceMap.find(id) };
            if (found != resourceMap.end()) {
                return found->getDataPointerCopy();
            }
            else {
                return std::make_shared(nullptr);
            }
        }

    private:
        std::shared_ptr<Resource<T>> getResourceOrThrow(const std::wstring& id) {

        }
    };

}