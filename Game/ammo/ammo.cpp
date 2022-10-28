#include "ammo.h"

namespace bugattack::ammo
{

AmmoBase::AmmoBase(UnitType type, class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    Unit{type, geometry}, target_{target}
{

}

std::shared_ptr<enemy::Enemy> AmmoBase::Target() const noexcept
{
    return target_;
}

}
