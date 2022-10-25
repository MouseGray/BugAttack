#pragma once

#include <cstdint>

#include "unit.h"

namespace bugattack
{
class Field;
}

class Enemy : public Unit
{
public:
    Enemy(UnitType type, class Geometry geometry, int level);

    void Move(const bugattack::Field& field, float seconds);

    void Damage(float damage) noexcept;

    bool IsLive() const noexcept;

    bool InEnd() const noexcept;

    inline int Cost() const noexcept { return cost_; }
private:
    float velocity_;
    float health_;
    bool in_end_;
    int cost_;
};
