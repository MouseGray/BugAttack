#include "tower.h"

namespace bugattack::tower
{

TowerBase::TowerBase(UnitType type, class Geometry geometry) :
    Unit{type, geometry}, reloading_{}
{

}

int Cost(UnitType type) noexcept
{
    switch (type)
    {
    case UnitType::Gun:
        return 75;
    case UnitType::Laser:
        return 200;
    case UnitType::RocketGun:
        return 300;
    default:
        assert(false && "Invalid tower type");
    }
    return 0;
}

}
