#pragma once

#include "enemy.h"

namespace bugattack::enemy
{

class Ant : public Enemy
{
public:
    static constexpr auto BASE_VELOCITY = 60.0f;
    static constexpr auto BASE_HEALTH   = 100;
    static constexpr auto COEF_VELOCITY = 2.0f;
    static constexpr auto COEF_HEALTH   = 20;

    Ant(class Geometry geometry, int level);
};

}
