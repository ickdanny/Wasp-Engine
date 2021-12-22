#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Resource.h"

namespace resource {

    template <typename T>
    class ResourceStorage {
        using ResourceMap = std::unordered_map<std::string&, Resource<T>>;

    private:
        ResourceMap resourceMap{};

    public:
        ResourceStorage() = default;
        virtual ~ResourceStorage() = default;

        virtual std::shared_ptr<T> get(const std::string& id) {
            auto found{ resourceMap.find(id) };
            if (found != resourceMap.end()) {
                return found->getDataPointerCopy();
            }
            else {
                return std::make_shared(nullptr);
            }
        }
        virtual void unload(const std::string& id) = 0;
        virtual void reload(const std::string& id) = 0;
        virtual void remove(const std::string& id) = 0;
        virtual void write(const std::string& id) const = 0;
    };
}