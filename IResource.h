#pragma once

#include <string>

//methods necessary for the resource tree
namespace resource {
    class IResource {
    public:
        virtual ~IResource() = default;
        virtual void unloadData() = 0;
        virtual void reloadData() = 0;
        virtual void writeData() = 0;
    };
}