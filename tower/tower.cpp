#include "tower.h"

namespace tower
{

TowerBase::TowerBase(UnitType type, class Geometry geometry) :
    Unit{type, geometry}, reloading_{}
{

}

}
