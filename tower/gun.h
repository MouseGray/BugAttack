#pragma once

#include <ammo/shot.h>

#include "tower.h"

namespace tower
{

class Gun final : public TowerBase
{
    static constexpr float RADIUS     = 100;
    static constexpr float MAX_RELOAD = 0.5f;
public:
    static constexpr int   COST       = 100;

    Gun(class Geometry geometry);

    void Update(const std::vector<std::shared_ptr<Enemy>> &enemies, float time) override;

    float Radius() const override;

    std::vector<ammo::Shot>::const_iterator begin() const noexcept;

    std::vector<ammo::Shot>::const_iterator end() const noexcept;
private:
    std::vector<ammo::Shot> ammo_;
};

}
