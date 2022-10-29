#include "tower.h"

namespace bugattack::tower
{

TowerBase::TowerBase(UnitType type, class Geometry geometry) :
    Unit{type, std::move(geometry)}, reloading_{}
{

}

void TowerBase::Reload(float time) noexcept
{
    if(reloading_ > 0.0f)
        reloading_ -= time;
}

bool TowerBase::Shoot(std::shared_ptr<enemy::Enemy> target) noexcept
{
    (void)target;

    if(reloading_ > 0.0f)
        return false;

    reloading_ = MaxRealodTime();
    return true;
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
