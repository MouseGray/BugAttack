#include "unit.h"

Unit::Unit(UnitType type, class Geometry geometry) :
    type_{type}, geometry_{geometry}
{

}

UnitType Unit::Type() const noexcept
{
    return type_;
}

const class Geometry& Unit::Geometry() const noexcept
{
    return geometry_;
}
