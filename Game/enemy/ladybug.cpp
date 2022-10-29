#include "ladybug.h"

namespace bugattack::enemy
{

Ladybug::Ladybug(class Geometry geometry, int level) :
    Enemy(UnitType::Bug2, geometry, level)
{
    velocity_ = enemy::Velocity<Ladybug>(level);
    health_ = Health<Ladybug>(level);
}

std::shared_ptr<Enemy> Ladybug::Share() const
{
    return const_cast<Ladybug&>(*this).shared_from_this();
}

int Cost(const Ladybug& ladybug) noexcept
{
    return ladybug.IsDestroyed() ? 100 : 0;
}

}
