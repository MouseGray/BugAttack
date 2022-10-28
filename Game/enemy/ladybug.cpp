#include "ladybug.h"

namespace bugattack::enemy
{

Ladybug::Ladybug(class Geometry geometry, int level) :
    Enemy(UnitType::Bug2, geometry, level)
{
    velocity_ = enemy::Velocity<Ladybug>(level);
    health_ = Health<Ladybug>(level);
}

}
