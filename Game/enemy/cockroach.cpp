#include "cockroach.h"

namespace bugattack::enemy
{

Cockroach::Cockroach(class Geometry geometry, int level) :
    Enemy(UnitType::Bug3, geometry, level)
{
    velocity_ = enemy::Velocity<Cockroach>(level);
    health_ = Health<Cockroach>(level);
}

std::shared_ptr<Enemy> Cockroach::Share() const
{
    return const_cast<Cockroach&>(*this).shared_from_this();
}

int Cost(const Cockroach& cockroach) noexcept
{
    return cockroach.IsDestroyed() ? 60 : 0;
}

}
