#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace wasp::game::components {
    enum class ScriptInstructions {
        error,                      //throws an error if reached

        timer,                      //stalls until a timer has finished <int, ticker>
        removeVisible,              //makes the entity invisible; removes VisibleMarker
        shiftOpacityPeriod,         //Sets opacity to the target value over a period
                                    //of time
                                    //todo: (target opacity, ticks) 
                                    //<std::tuple<float, int>, float>
        shiftScalePeriod,           //Sets scale to the target value over a period
                                    //of time
                                    //todo: (target scale, ticks)
                                    //<std::tuple<float, int>, float>
        setSpriteInstruction,       //Sets the sprite instruction component
                                    //<SpriteInstruction, Void>
        setAnimation,               //Sets the animation component, <Animation, Void>
        setDrawOrder,               //Sets the draw order component, <DrawOrder, Void>

        waitForNoSpawns,            //stalls until the entity has no active spawns
        
        /*
        public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_FOCUSED = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_UNFOCUSED = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_NORMAL = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_BOMBING = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_DEAD = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_RESPAWNING = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_PLAYER_RESPAWN_INVULNERABLE = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_BOSS_DEATH = new Instructions<>();
    public static final Instructions<Void, Void> WAIT_UNTIL_DIALOGUE_OVER = new Instructions<>();
    */
        
        //todo: how do these work? maybe same as inbound/outbound? ie relative to edge
        boundaryYLow,               //stalls until the entity's y coordinate is below
                                    //a stored value
                                    //<float, Void> (boundary value)
        boundaryYHigh,              //stalls until the entity's y coordinate is above
                                    //a stored value
                                    //<float, Void> (boundary value)
        boundaryXLow,               //stalls until the entity's x coordinate is below
                                    //a stored value
                                    //<float, Void> (boundary value)
        boundaryXHigh,              //stalls until the entity's x coordinate is above
                                    //a stored value
                                    //<float, Void> (boundary value)
        boundaryY,                  //stalls until the entity's y coordinate is above
                                    //or below a stored value
                                    //<float, Void> (boundary value)
        boundaryX,                  //stalls until the entity's x coordinate is above
                                    //or below a stored value
                                    //<float, Void> (boundary value)
        setCollidable,              //makes the entity collidable; adds CollidableMarker
        removeCollidable,           //makes the entity uncollidable; removes marker

        setHealth,                  //sets the entity's health, <int, Void> (hp)
        removeHealth,               //removes the entity's health component

        setDamage,                  //sets the entity's damage, <int, Void> (dmg)
        removeDamage,               //removes the entity's damage component

        setSpawn,                   //sets the entity's spawn, <SpawnProgram, Void>
        addSpawn,                   //adds a spawn to the entity, <SpawnProgram, Void>
        clearSpawn,                 //clears the entity's spawns

        setVelocity,                //sets the entity's velocity, <Velocity, Void>
        setVelocityToPlayer,        //sets the entity's velocity in the direction of
                                    //the player
                                    //<float, Void> (speed)
        setRandomVelocity,          //sets the entity's velocity to a UNIFORMLY chosen
                                    //random velocity
                                    //<std::tuple<float, float, float, float>, Void>
                                    //(speedLow, speedHigh, angleLow, angleHigh)
        
        //public static final Instructions<Void, Void> REMOVE_VELOCITY

        setInbound,                 //sets the entity's inbound <Inbound, Void>
        removeInbound,              //removes the entity's inbound component

        setOutbound,                //sets the entity's outbound <Outbound, Void>

        //todo: slow to halt -> velocity period?
        /*

    public static final Instructions<Integer, Double> SLOW_TO_HALT = new Instructions<>();
    //velocity, ticks
    public static final Instructions<Tuple2<AbstractVector, Integer>, Double> SPEED_UP_TO_VELOCITY = new Instructions<>();
    //velocity, initAngle, ticks
    public static final Instructions<Tuple3<AbstractVector, Angle, Integer>, Tuple2<Double, Double>>
            SPEED_UP_AND_TURN_TO_VELOCITY = new Instructions<>();
    public static final Instructions<Tuple3<AbstractVector, Angle, Integer>, Tuple2<Double, Double>>
            SPEED_UP_AND_TURN_TO_VELOCITY_LONG_ANGLE = new Instructions<>();
    //velocity, ticks
    public static final Instructions<Tuple2<AbstractVector, Integer>, Double> SLOW_DOWN_TO_VELOCITY = new Instructions<>();
    public static final Instructions<Tuple2<AbstractVector, Integer>, Tuple2<Double, Double>>
            SLOW_DOWN_AND_TURN_TO_VELOCITY = new Instructions<>();
    public static final Instructions<Tuple2<AbstractVector, Integer>, Tuple2<Double, Double>>
            SLOW_DOWN_AND_TURN_TO_VELOCITY_LONG_ANGLE = new Instructions<>();

    //angle, ticks
    public static final Instructions<Tuple2<Angle, Integer>, Double> TURN_TO = new Instructions<>();
    public static final Instructions<Tuple2<Angle, Integer>, Double> TURN_TO_LONG_ANGLE = new Instructions<>();

    //target speed, acceleration
    public static final Instructions<Tuple2<Double, Double>, Void> ACCELERATE_TO_SPEED = new Instructions<>();
    //target speed, deceleration
    public static final Instructions<Tuple2<Double, Double>, Void> DECELERATE_TO_SPEED = new Instructions<>();

    //target position, maximum speed, storing init distance
    public static final Instructions<Tuple2<DoublePoint, Double>, Double> GOTO_DECELERATING = new Instructions<>();
    //bounds, minimum radius, maximum radius, maximum speed, storing target and init distance
    public static final Instructions<Tuple4<AABB, Double, Double, Double>, Tuple2<DoublePoint, Double>>
            BOUND_RADIUS_GOTO_DECELERATING = new Instructions<>();

    public static final Instructions<Void, Void> FOLLOW_PLAYER = new Instructions<>();

    public static final Instructions<Void, Void> CLEAR_FIELD = new Instructions<>();
    public static final Instructions<Void, Void> CLEAR_FIELD_LONG = new Instructions<>();

    public static final Instructions<Void, Void> DIE = new Instructions<>();
    public static final Instructions<Void, Void> REMOVE_ENTITY = new Instructions<>();

    public static final Instructions<String, Void> START_TRACK = new Instructions<>();

    public static final Instructions<String, Void> SHOW_DIALOGUE = new Instructions<>();

    public static final Instructions<Void, Void> NEXT_STAGE = new Instructions<>();

    public static final Instructions<Void, Void> GAME_OVER = new Instructions<>();
    public static final Instructions<Void, Void> GAME_WIN = new Instructions<>();
    */

        
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

        //Constructs a script node with the given scriptInstruction.
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

        //Constructs a script node with the given scriptInstruction and internal data
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