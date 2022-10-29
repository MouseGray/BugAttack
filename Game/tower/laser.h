#pragma once

#include <optional>

#include <ammo/laserray.h>

#include <enemy/enemy.h>

#include "tower.h"

namespace bugattack::tower
{

class Laser final : public TowerBase
{
    static constexpr float RADIUS     = 150;
    static constexpr float MAX_RELOAD = 3.0f;
public:
    Laser(class Geometry geometry);

    std::optional<ammo::LaserRay>& Ray() noexcept;

    const std::optional<ammo::LaserRay>& Ray() const noexcept;

    void ValidateAmmo() noexcept;

    bool IsWork() const noexcept;

    float MaxRealodTime() const override;

    bool Shoot(std::shared_ptr<enemy::Enemy> target) noexcept override;

    float Radius() const override;
private:
    std::optional<ammo::LaserRay> ammo_;
};

template<typename IIt>
void Update(Laser& laser, IIt begin, IIt end, float time)
{
    laser.Reload(time);

    if(laser.Ray())
    {
        ammo::Update(*laser.Ray(), time);
        laser.ValidateAmmo();
    }

    if(laser.IsWork())
    {
        const auto& target = laser.Ray()->Target();
        laser.SetGeometry(LookAt(laser.Geometry(), target.Geometry()));
        return;
    }

    auto target = FindTarget(laser, begin, end);

    if(!target)
        return;

    laser.SetGeometry(LookAt(laser.Geometry(), target->Geometry()));

    laser.Shoot(target);
}

}
