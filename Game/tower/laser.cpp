#include "laser.h"

#include <enemy/enemy.h>

namespace bugattack::tower
{

Laser::Laser(class Geometry geometry) :
    TowerBase{UnitType::Laser, std::move(geometry)}
{

}

float Laser::MaxRealodTime() const
{
    return MAX_RELOAD;
}

bool Laser::Shoot(std::shared_ptr<enemy::Enemy> target) noexcept
{
    if(!TowerBase::Shoot(target))
        return false;

    ammo_.emplace(geometry_, std::move(target));
    return true;
}

float Laser::Radius() const
{
    return RADIUS;
}

std::optional<ammo::LaserRay>& Laser::Ray() noexcept
{
    return ammo_;
}

const std::optional<ammo::LaserRay> &Laser::Ray() const noexcept
{
    return ammo_;
}

void Laser::ValidateAmmo() noexcept
{
    if(ammo_->IsDestroyed())
        ammo_.reset();
}

bool Laser::IsWork() const noexcept
{
    return ammo_.has_value();
}

}
