#pragma once

#include <ammo/rocket.h>

#include "tower.h"

namespace tower
{

class RocketGun final : public TowerBase
{
    static constexpr float RADIUS     = 300;
    static constexpr float MAX_RELOAD = 5.0f;
public:
    static constexpr float COST       = 300;

    RocketGun(class Geometry geometry);

    void Update(const std::vector<std::shared_ptr<Enemy>> &enemies, float time) override;

    float Radius() const override;

    std::vector<ammo::Rocket>::const_iterator begin() const noexcept;

    std::vector<ammo::Rocket>::const_iterator end() const noexcept;
private:
    std::vector<ammo::Rocket> ammo_;
};

}
