#pragma once

#include <memory>

#include <unit.h>

class Enemy;

namespace ammo
{

class AmmoBase : public Unit, public IDestroyable
{
public:
    AmmoBase(UnitType type, class Geometry geometry, std::shared_ptr<Enemy> target);

    virtual void Update(float time) = 0;

    std::shared_ptr<Enemy> Enemy() const noexcept;

    virtual ~AmmoBase() = default;
protected:
    std::shared_ptr<class Enemy> target_;
};

}
