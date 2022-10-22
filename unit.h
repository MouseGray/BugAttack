#pragma once

#include "primitives.h"

enum class UnitType
{
    Bug1,
    Bug2,
    Bug3,
    Gun,
    Laser,
    RocketGun,
    Rocket,
    LaserRay,
    Shot,
    Boom
};

struct Unit
{
    UnitType type_;
    Point    position_;
    Degree   rotation_;
};
