#include "ant.h"

namespace bugattack::enemy
{

Ant::Ant(class Geometry geometry, int level) :
    Enemy(UnitType::Bug1, geometry, level)
{
    velocity_ = enemy::Velocity<Ant>(level);
    health_ = Health<Ant>(level);
}

}
