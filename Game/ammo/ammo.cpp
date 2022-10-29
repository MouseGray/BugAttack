#include "ammo.h"

namespace bugattack::ammo
{

AmmoBase::AmmoBase(UnitType type, class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    Unit{type, std::move(geometry)}, target_{target}
{

}

enemy::Enemy& AmmoBase::Target() const noexcept
{
    return *target_;
}

}
