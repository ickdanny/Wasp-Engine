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

        spawn,              //cast the node data to a ComponentTupleBaseSharedPtr
        spawnPos,           //spawn at [posNode]
        spawnPosVel,        //spawn with [posNode, velNode]

        value,              //returns a stored value
        valueDifficulty,    //returns a stored value based on the difficulty

        add,                //adds the result of evaluating two nodes [node1, node2]
        max,                //returns the max of evaluating two nodes [node1, node2]
        uniformRandom,      //returns a uniformly random value
                            //float: [minNode, maxNode]

        //predicates
        tickMod,            //returns (tick + int1) % int2 == 0  [intNode1, intNode2]
        lastTick,           //returns tick == 1
        isPlayerFocused,    //returns true if the focus gameCommand is found

        //float
        entityX,            //returns the x coordinate of the entity
        entityY,            //returns the y coordinate of the entity

        //point2
        entityPosition,     //returns the position of the entity
        entityOffset,       //applies an offset to the position of the entity 
                            //[offsetNode] (which is a Velocity node)

        //velocity
        velocityFromPolar,  //returns the result of creating a velocity from its nodes
                            //[magnitudeNode, angleNode]

        //formations
        mirrorFormation,    //reflects over y axis 
                            //[posNode, velNode, axisNode, posVelConsumerNode]
        mirrorPosFormation, //reflects over y axis; doesn't affect velocity
                            //[posNode, axisNode, posConsumerNode]
        arcFormation,       //creates an arc with the given symmetry and increment
                            //[velNode, symmetryNode, incrementNode, velConsumerNode]
        ringFormation,      //creates a ring with the given symmetry
                            //[velNode, symmetryNode, velConsumerNode]
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
        SpawnNode& link(const std::shared_ptr<SpawnNode>& node, Ts... args) {
            linkedNodePointers.push_back(node);
            if constexpr (sizeof...(args) > 0) {
                return link(args...);
            }
            return *this;
        }
    };

    template <typename... Ts>
    struct SpawnNodeData : SpawnNode {
        //fields
        std::tuple<Ts...> data{};

        //Constructs a spawn node with the given spawnInstruction and data
        SpawnNodeData(SpawnInstructions spawnInstruction, const Ts&... args)
            : SpawnNode{ spawnInstruction }
            , data{ args... } {
        }

        ~SpawnNodeData() override = default;
    };

	struct SpawnProgram : public utility::Ticker {
        //fields
        std::shared_ptr<SpawnNode> baseSpawnNodePointer{};

        //Constructs a SpawnProgram with the given baseSpawnNodePointer and tick data
        SpawnProgram(
            const std::shared_ptr<SpawnNode>& baseSpawnNodePointer,
            int maxTick,
            bool looping
        )
            : Ticker{ maxTick, looping }
            , baseSpawnNodePointer{ baseSpawnNodePointer } {
        }
	};

    using SpawnProgramList = std::vector<SpawnProgram>;
}