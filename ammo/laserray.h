#pragma once

#include "ammo.h"

namespace ammo
{

class LaserRay : public AmmoBase
{
    static constexpr uint32_t DAMAGE        = 50;
    static constexpr float    SPEED         = 0.0f;
    static constexpr float    MAX_WORK_TIME = 1.0f;
public:
    LaserRay(class Geometry geometry, std::shared_ptr<class Enemy> target);

    void Update(float time) override;

    bool IsDestroyed() const override;
private:
    float work_time_;
};

}
