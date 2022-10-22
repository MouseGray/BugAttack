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
    Enemy(UnitType type, Point pos, Degree angle, int level);

    void Move(const bugattack::Field& field, Time seconds);

    void Damage(float damage) noexcept;

    bool IsLive() const noexcept;

    bool InEnd() const noexcept;

    inline Point Position() const noexcept { return position_; }

    inline ID EnemyID() const noexcept { return id_; }
private:
    static ID GetID() noexcept;

    ID id_;
    float velocity_;
    float health_;
    bool in_end_;
};
