#include "utils.h"

#include <glm/gtx/vector_angle.hpp>

namespace utils
{

glm::vec2 Direction(float angle) noexcept
{
    return glm::rotate(glm::vec2{1.0f, 0.0f}, glm::radians(angle));
}

glm::vec2 Direction(glm::vec2 from, glm::vec2 to) noexcept
{
    return glm::normalize(to - from);
}

float Angle(glm::vec2 direction, glm::vec2 vector_null) noexcept
{
    return -glm::degrees(glm::orientedAngle(direction, vector_null));
}

bool IsCodirectional(glm::vec2 lhs, glm::vec2 rhs) noexcept
{
    return glm::normalize(lhs) == glm::normalize(rhs);
}

bool IsEqual(glm::vec2 lhs, glm::vec2 rhs) noexcept
{
    return glm::all(glm::equal(lhs, rhs));
}

bool InRange(glm::vec2 obj, glm::vec2 point, float radius) noexcept
{
    return glm::all(glm::equal(obj, point, radius));
}



}
