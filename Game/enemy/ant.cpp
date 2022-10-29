#include "ant.h"

namespace bugattack::enemy
{

Ant::Ant(class Geometry geometry, int level) :
    Enemy(UnitType::Bug1, geometry, level)
{
    velocity_ = enemy::Velocity<Ant>(level);
    health_ = Health<Ant>(level);
}

std::shared_ptr<Enemy> Ant::Share() const
{
    return const_cast<Ant&>(*this).shared_from_this();
}

int Cost(const Ant& ant) noexcept
{
    return ant.IsDestroyed() ? 30 : 0;
}

}
