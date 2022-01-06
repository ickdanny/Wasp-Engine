#pragma once

#include <string>

#include "UnsupportedOperationError.h"

namespace wasp::resource {

    using exception::UnsupportedOperationError;

    class IResourceStorage {
    public:
        IResourceStorage() = default;
        virtual ~IResourceStorage() = default;

        virtual void unload(const std::wstring& id) = 0;
        virtual void remove(const std::wstring& id) = 0;
        virtual void reload(const std::wstring& id) = 0;
        virtual void write(const std::wstring& id) const {
            throw UnsupportedOperationError{ "Error resource write unsupported" };
        };
    };
}