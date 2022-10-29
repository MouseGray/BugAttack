#pragma once

#include <memory>

#include <unit/unit.h>

namespace bugattack::enemy{class Enemy;}

namespace bugattack::ammo
{

class AmmoBase : public Unit, public IDestroyable
{
public:
    AmmoBase(UnitType type, class Geometry geometry, std::shared_ptr<enemy::Enemy> target);

    enemy::Enemy& Target() const noexcept;

    virtual float Damage() const = 0;

    virtual float Velocity() const = 0;
protected:
    virtual ~AmmoBase() = default;

    std::shared_ptr<enemy::Enemy> target_;
};

}
