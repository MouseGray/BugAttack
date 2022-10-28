#pragma once

#include "enemy.h"

namespace bugattack::enemy
{

class Ladybug : public Enemy
{
public:
    static constexpr auto BASE_VELOCITY = 40.0f;
    static constexpr auto BASE_HEALTH   = 200;
    static constexpr auto COEF_VELOCITY = 2.0f;
    static constexpr auto COEF_HEALTH   = 40;

    Ladybug(class Geometry geometry, int level);
};

}
