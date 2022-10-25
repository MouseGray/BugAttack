#pragma once

#include "ammo.h"

namespace ammo
{

class Shot : public AmmoBase
{
    static constexpr uint32_t DAMAGE          = 10;
    static constexpr float    SPEED           = 150.0f;
    static constexpr float    DAMAGE_DISTANCE = 1.5f;
public:
    Shot(class Geometry geometry, std::shared_ptr<class Enemy> target);

    void Update(float time) override;

    bool IsDestroyed() const override;
};

}
