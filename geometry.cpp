#include "geometry.h"

#include "utils.h"
#include <iostream>
#include <ostream>

Geometry LookAt(const Geometry& from, const Geometry& to) noexcept
{
    if(from.position == to.position) return from;
    auto direction = utils::Direction(from.position, to.position);
    return {from.position, utils::Angle(direction)};
}

Geometry Move(const Geometry& geometry, float distance) noexcept
{
    auto direction = utils::Direction(geometry.rotation);
    return {geometry.position + direction*distance, geometry.rotation};
}

Geometry MoveTo(const Geometry &from, const Geometry &to, float distance) noexcept
{
    return Move(LookAt(from, to), distance);
}

float Distance(const Geometry &from, const Geometry &to) noexcept
{
    return glm::distance(from.position, to.position);
}
