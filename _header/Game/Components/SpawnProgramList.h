#pragma once

#include <vector>
#include <memory>

#include "Utility/Ticker.h"

namespace wasp::game::components {

    enum class SpawnInstructions {
        error,              //throws an error if reached
        list,               //sequential list of instructions [node1, node2...]
        condition,          //if statement [predicateNode, trueNode]
        conditionElse,      //if-else statement [predicateNode, trueNode, falseNode]
        playerPowerSplit,   //chooses node based on player power [0Node, 8Node...]
        spawn,              //cast the node data to a ComponentTupleSharedPtr
        //todo: implement spawnPos and spawnPosVel
        spawnPos,           //cast the node data with [posNode]
        spawnPosVel,        //cast the node data with [posNode, velNode]

        value,              //returns a stored value
        valueDifficulty,    //returns a stored value based on the difficulty

        //predicates
        tickMod,            //returns (tick + int1) % int2 == 0  [intNode1, intNode2]
        lastTick,           //returns tick == 1
        isPlayerFocused,    //returns true if the focus gameCommand is found

        numInstructions
    };

    struct SpawnNode {
        //fields
        SpawnInstructions spawnInstruction{};
        std::vector<std::shared_ptr<SpawnNode>> linkedNodePointers{};

        //Constructs a spawn node with the given spawnInstruction.
        SpawnNode(SpawnInstructions spawnInstruction)
            : spawnInstruction{ spawnInstruction } {
        }

        //virtual destructor
        virtual ~SpawnNode() = default;

        template <typename... Ts>
        void link(const std::shared_ptr<SpawnNode>& node, Ts... args) {
            linkedNodePointers.push_back(node);
            if constexpr (sizeof...(args) > 0) {
                link(args...);
            }
        }
    };

    template <typename... Ts>
    struct SpawnNodeData : SpawnNode {
        //fields
        std::tuple<Ts...> data{};

        //Constructs a spawn node with the given spawnInstruction and data
        SpawnNodeData(SpawnInstructions spawnInstruction, const Ts& args...)
            : SpawnNode{ spawnInstruction }
            , data{ args } {
        }

        ~SpawnNodeData() override = default;
    };

	struct SpawnProgram : public utility::Ticker {
        //fields
        std::shared_ptr<SpawnNode> baseSpawnNodePointer{};

        //Constructs a SpawnProgram with the given baseSpawnNodePointer and tick data
        SpawnProgram(
            const std::shared_ptr<SpawnNode>& baseSpawnNode,
            int maxTick,
            bool looping
        )
            : Ticker{ maxTick, looping }
            , baseSpawnNodePointer{ baseSpawnNodePointer } {
        }
	};

    using SpawnProgramList = std::vector<SpawnProgram>;
}