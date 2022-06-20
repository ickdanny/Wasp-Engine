#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace wasp::game::components {
    enum class ScriptInstructions {
        error,              //throws an error if reached
        

        numInstructions
    };

    struct ScriptNode {
    private:
        //typedefs
        using ScriptNodeSharedPointer = std::shared_ptr<ScriptNode>;
        using OldToNewMapType = std::unordered_map<ScriptNode*, ScriptNode*>;

    public:
        //fields
        ScriptInstructions scriptInstruction{};
        std::vector<ScriptNodeSharedPointer> linkedNodePointers{};

        //Constructs a spawn node with the given spawnInstruction.
        ScriptNode(ScriptInstructions scriptInstruction)
            : scriptInstruction{ scriptInstruction } {
        }

        //virtual destructor
        virtual ~ScriptNode() = default;

        template <typename... Ts>
        ScriptNode& link(const ScriptNodeSharedPointer& node, Ts... args) {
            linkedNodePointers.push_back(node);
            if constexpr (sizeof...(args) > 0) {
                return link(args...);
            }
            return *this;
        }

    protected:
        virtual void clearData(void* voidPointer) {}

        friend struct ScriptProgram;
    };

    //internal data should not be changed
    template <typename Internal, typename External>
    struct ScriptNodeData : ScriptNode {
        //fields
        Internal internalData{};

        //Constructs a spawn node with the given spawnInstruction and internal
        ScriptNodeData(
            ScriptInstructions scriptInstruction, 
            const Internal& internalData
        )
            : ScriptNode{ scriptInstruction }
            , internalData{ internalData } {
        }

        ~ScriptNodeData() override = default;

    protected:
        void clearData(void* voidPointer) override {
            if (voidPointer) {
                delete reinterpret_cast<External*>(voidPointer);
            }
        }
    };

    struct ScriptProgram {
        //fields
        std::shared_ptr<ScriptNode> currentNodePointer{};
        std::unordered_map<ScriptNode*, void*> externalData{};

        //Constructs a SpawnProgram with the given currentNodePointer 
        ScriptProgram(
            const std::shared_ptr<ScriptNode>& currentNodePointer
        )
            : currentNodePointer { currentNodePointer } {
        }

        //clears the external data
        ~ScriptProgram() {
            for (auto [scriptNodePointer, voidPointer] : externalData) {
                scriptNodePointer->clearData(voidPointer);
            }
        }
    };

    using ScriptProgramList = std::vector<ScriptProgram>;
}