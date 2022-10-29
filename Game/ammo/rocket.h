#pragma once

#include "ammo.h"

namespace bugattack::ammo
{

class Rocket : public AmmoBase
{
    static constexpr auto DAMAGE        = 70;
    static constexpr auto VELOCITY      = 100.0f;
    static constexpr auto MAX_BOOM_TIME = 1.5f;
    static constexpr auto BOOM_SPEED    = 7.0f;
public:
    static constexpr auto DAMAGE_DISTANCE = 1.5f;

    Rocket(class Geometry geometry, std::shared_ptr<enemy::Enemy> target);

    void Boom() noexcept;

    void AddBoomTime(float time) noexcept;

    bool IsBoom() const noexcept;

    float BoomTime() const noexcept;

    float MaxBoomTime() const noexcept;

    float Damage() const override;

    float Velocity() const override;

    bool IsDestroyed() const override;
private:
    float boom_time_;
};

void Update(Rocket& rocket, float time) noexcept;

}
