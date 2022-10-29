#pragma once

#include "ammo.h"

namespace bugattack::ammo
{

class LaserRay : public AmmoBase
{
    static constexpr auto DAMAGE        = 50;
    static constexpr auto VELOCITY      = 0.0f;
    static constexpr auto MAX_WORK_TIME = 1.0f;
public:
    LaserRay(class Geometry geometry, std::shared_ptr<enemy::Enemy> target);

    void AddWorkTime(float time) noexcept;

    float Damage() const override;

    float Velocity() const override;

    bool IsDestroyed() const override;
private:
    float work_time_;
};

void Update(LaserRay& ray, float time) noexcept;

}
