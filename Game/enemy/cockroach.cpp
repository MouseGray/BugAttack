#include "cockroach.h"

namespace bugattack::enemy
{

Cockroach::Cockroach(class Geometry geometry, int level) :
    Enemy(UnitType::Bug3, geometry, level)
{
    velocity_ = enemy::Velocity<Cockroach>(level);
    health_ = Health<Cockroach>(level);
}

}
