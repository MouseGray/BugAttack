#include "gun.h"

#include <algorithm>

namespace bugattack::tower
{

Gun::Gun(class Geometry geometry) :
    TowerBase{UnitType::Gun, std::move(geometry)}
{

}

bool Gun::Shoot(std::shared_ptr<enemy::Enemy> target) noexcept
{
    if(!TowerBase::Shoot(target))
        return false;

    ammo_.emplace_back(geometry_, std::move(target));
    return true;
}

void Gun::ValidateAmmo() noexcept
{
    ammo_.erase(std::remove_if(ammo_.begin(), ammo_.end(), IsDestroyed{}), ammo_.end());
}

float Gun::Radius() const
{
    return RADIUS;
}

float Gun::MaxRealodTime() const
{
    return MAX_RELOAD;
}

std::vector<ammo::Shot>::iterator Gun::begin() noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Shot>::iterator Gun::end() noexcept
{
    return ammo_.end();
}

std::vector<ammo::Shot>::const_iterator Gun::begin() const noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Shot>::const_iterator Gun::end() const noexcept
{
    return ammo_.end();
}

}
