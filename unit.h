#pragma once

#include "primitives.h"

enum class UnitType : int
{
//  Enemies     Towers      Ammo        Other
    Bug1,       Gun,        Shot,       Boom,
    Bug2,       Laser,      LaserRay,
    Bug3,       RocketGun,  Rocket
};

class Unit
{
    friend class Render;
public:
    Unit(UnitType type, Point position, Degree rotation) noexcept;

    virtual ~Unit();// = 0;
protected:
    UnitType type_;
    Point    position_;
    Degree   rotation_;
};
