#pragma once

#include "enemy.h"

namespace bugattack::enemy
{

class Cockroach final : public Enemy
{
public:
    static constexpr auto BASE_VELOCITY = 47.0f;
    static constexpr auto BASE_HEALTH   = 150;
    static constexpr auto COEF_VELOCITY = 3.0f;
    static constexpr auto COEF_HEALTH   = 30;

    Cockroach(class Geometry geometry, int level);

    std::shared_ptr<Enemy> Share() const override;
};

int Cost(const Cockroach& cockroach) noexcept;

}
