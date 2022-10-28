#pragma once

#include <utils/geometry.h>

namespace bugattack
{

enum class UnitType : int
{
//  Enemies     Towers      Ammo        Other
    Bug1,       Gun,        Shot,       Boom,
    Bug2,       Laser,      LaserRay,   Map,
    Bug3,       RocketGun,  Rocket
};

class Unit
{
public:
    Unit(UnitType type, Geometry geometry);

    virtual ~Unit() = default;

    UnitType Type() const noexcept;

    const class Geometry& Geometry() const noexcept;

    void SetGeometry(class Geometry geometry) noexcept;
protected:
    UnitType type_;
    class Geometry geometry_;
};

struct IDestroyable
{
    IDestroyable() = default;
    virtual ~IDestroyable() = default;

    virtual bool IsDestroyed() const = 0;
};

struct IsDestroyed
{
    bool operator()(const IDestroyable& object)
    {
        return object.IsDestroyed();
    }
};

}
