#include "rocket.h"

#include <algorithm>

namespace bugattack::tower
{

RocketGun::RocketGun(class Geometry geometry) :
    TowerBase{UnitType::RocketGun, std::move(geometry)}
{

}

float RocketGun::MaxRealodTime() const
{
    return MAX_RELOAD;
}

float RocketGun::Radius() const
{
    return RADIUS;
}

bool RocketGun::Shoot(std::shared_ptr<enemy::Enemy> target) noexcept
{
    if(!TowerBase::Shoot(target))
        return false;

    ammo_.emplace_back(geometry_, std::move(target));
    return true;
}

void RocketGun::ValidateAmmo()
{
    ammo_.erase(std::remove_if(ammo_.begin(), ammo_.end(), IsDestroyed{}), ammo_.end());
}

std::vector<ammo::Rocket>::iterator RocketGun::begin() noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Rocket>::iterator RocketGun::end() noexcept
{
    return ammo_.end();
}

std::vector<ammo::Rocket>::const_iterator RocketGun::begin() const noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Rocket>::const_iterator RocketGun::end() const noexcept
{
    return ammo_.end();
}

}
