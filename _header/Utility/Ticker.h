#pragma once

namespace wasp::utility {

    class Ticker {
    private:
        //fields
        int tick{};
        int maxTick{};
        bool looping{};

    public:
        Ticker(int maxTick, bool looping) 
            : tick{ maxTick }
            , maxTick{ maxTick }
            , looping{ looping } {
        }

        //Returns <= 0 if over
        int stepAndGetTick() {
            int toRet = tick--;
            if (looping) {
                if (tick <= 0) {
                    tick = maxTick;
                }
            }
            return toRet;
        }

        void setLooping(bool looping) {
            this->looping = looping;
        }

        int getMaxTick() const {
            return maxTick;
        }
    };
}