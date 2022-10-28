#pragma once

#include "ammo.h"

namespace bugattack::ammo
{

class Rocket : public AmmoBase
{
    static constexpr uint32_t DAMAGE          = 70;
    static constexpr float    SPEED           = 100.0f;
    static constexpr float    DAMAGE_DISTANCE = 1.5f;
    static constexpr float    MAX_BOOM_TIME   = 1.5f;
    static constexpr float    BOOM_SPEED      = 7.0f;
public:
    Rocket(class Geometry geometry, std::shared_ptr<enemy::Enemy> target);

    void Update(float time) override;

    bool IsDestroyed() const override;

    float BoomTime() const noexcept;

    float MaxBoomTime() const noexcept;
private:
    float boom_time_;
};

}
