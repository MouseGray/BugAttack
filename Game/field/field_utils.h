#pragma once

#include <utils/geometry.h>
#include <utils/primitives.h>

#include "field.h"

namespace bugattack::field
{

constexpr float POINT_SIZE = 20.0f;

PURE glm::vec2 ToFieldPoint(const Geometry& geometry) noexcept;

PURE Geometry FromFieldPoint(const glm::ivec2& point) noexcept;

PURE Geometry Move(const Field& field, Geometry geometry, float distance) noexcept;

}
