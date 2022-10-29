#pragma once

#include "ammo.h"

namespace bugattack::ammo
{

class Shot : public AmmoBase
{
    static constexpr auto DAMAGE   = 10;
    static constexpr auto VELOCITY = 150.0f;
public:
    static constexpr float    DAMAGE_DISTANCE = 1.5f;

    Shot(class Geometry geometry, std::shared_ptr<enemy::Enemy> target);

    float Damage() const override;

    float Velocity() const override;

    bool IsDestroyed() const override;
};

void Update(Shot& shot, float time) noexcept;

}
