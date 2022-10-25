#pragma once

#include <optional>

#include <ammo/laserray.h>

#include "tower.h"

namespace tower
{

class Laser final : public TowerBase
{
    static constexpr float RADIUS     = 150;
    static constexpr float MAX_RELOAD = 3.0f;
public:
    static constexpr float COST       = 200;

    Laser(class Geometry geometry);

    void Update(const std::vector<std::shared_ptr<Enemy>> &enemies, float time) override;

    float Radius() const override;

    const std::optional<ammo::LaserRay>& Ray() const noexcept;
private:
    std::optional<ammo::LaserRay> ammo_;
};

}
