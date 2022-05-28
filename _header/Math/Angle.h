#pragma once

#include <string>

namespace wasp::math {

    //utility functions
    constexpr float toDegrees(float radians);
    constexpr float toRadians(float degrees);

    //The Angle class represents an angle from 0 to 360 degrees.
    class Angle {
    private:
        //fields
        mutable float angle{};

    public:
        Angle() = default;

        Angle(float degrees)
            : angle{ degrees } {
            modAngle();
        }

        //getters
        float getAngle() const {
            return angle;
        }
        float getAngleRadians() const;

        //setters
        void setAngle(float degrees);
        void setAngleRadians(float radians);

        float smallerDifference(const Angle& other) const;
        float largerDifference(const Angle& other) const;

        //returns the result of flipping this angle along the y axis
        Angle flipY() const;
        //returns the result of flipping this angle along the x axis
        Angle flipX() const;

        //unary negative operator
        Angle operator-() const;

        //mathematical assignment operators
        Angle& operator+=(float right);
        Angle& operator-=(float right);

        //conversion to float
        operator float() const {
            return angle;
        }
        //conversion to string
        explicit operator std::string() const;

    private:
        void modAngle() const;
    };

    //mathematical operators for Angle class
    bool operator==(const Angle& a, const Angle& b);
    bool operator!=(const Angle& a, const Angle& b);
    Angle operator+(const Angle& a, const Angle& b);
    Angle operator-(const Angle& a, const Angle& b);
}