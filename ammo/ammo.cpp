#include "ammo.h"

namespace ammo
{

AmmoBase::AmmoBase(UnitType type, class Geometry geometry, std::shared_ptr<class Enemy> target) :
    Unit{type, geometry}, target_{target}
{

}

std::shared_ptr<class Enemy> AmmoBase::Enemy() const noexcept
{
    return target_;
}

}
