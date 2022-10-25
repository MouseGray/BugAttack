#include "utils.h"

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

float Angle(glm::vec2 direction, glm::vec2 vector_null)
{
    return -glm::degrees(glm::orientedAngle(direction, vector_null));
}

}
