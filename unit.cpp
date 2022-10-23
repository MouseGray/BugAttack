#include "unit.h"

Unit::Unit(UnitType type, Point position, Degree rotation) noexcept :
    type_{type}, position_{position}, rotation_{rotation}
{

}

Unit::~Unit()
{

}
