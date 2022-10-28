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

    virtual void Update(float time) = 0;

    std::shared_ptr<enemy::Enemy> Target() const noexcept;

    virtual ~AmmoBase() = default;
protected:
    std::shared_ptr<enemy::Enemy> target_;
};

}
