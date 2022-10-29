#include "game_render.h"

#include <tower/gun.h>
#include <tower/laser.h>
#include <tower/rocket.h>

#include <ammo/shot.h>
#include <ammo/laserray.h>
#include <ammo/rocket.h>

#include <enemy/enemy.h>

#include <utils/utils.h>

#include "map.h"

#include "render.h"

namespace bugattack
{

void Render(class Render& render, const bugattack::ammo::Shot& shot)
{
    render.DrawUnit(shot);
}

void Render(class Render& render, const bugattack::ammo::LaserRay& ray)
{
    render.DrawLine(ray.Geometry().position.x,
                    ray.Geometry().position.y,
                    ray.Target().Geometry().position.x,
                    ray.Target().Geometry().position.y,
                    glm::vec4{1.0, 0.0, 0.0, 0.7f});
}

void Render(class Render& render, const bugattack::ammo::Rocket& rocket)
{
    if(rocket.Type() == bugattack::UnitType::Boom)
        render.DrawUnit(rocket, std::min(rocket.BoomTime(), rocket.MaxBoomTime())*30.0f);
    else
        render.DrawUnit(rocket);
}

void Render(class Render& render, const bugattack::tower::Gun& gun)
{
    for(auto&& ammo: gun)
        Render(render, ammo);

    render.DrawUnit(gun);
}

void Render(class Render& render, const bugattack::tower::Laser& laser)
{
    const auto& ray = laser.Ray();
    if(ray)
        Render(render, *ray);

    render.DrawUnit(laser);
}

void Render(class Render& render, const bugattack::tower::RocketGun& rocket_gun)
{
    for(auto&& ammo: rocket_gun)
        Render(render, ammo);

    render.DrawUnit(rocket_gun);
}

void Render(class Render& render, const bugattack::tower::TowerBase& tower)
{
    switch (tower.Type())
    {
        case bugattack::UnitType::Gun:
            Render(render, static_cast<const bugattack::tower::Gun&>(tower));
            break;
        case bugattack::UnitType::Laser:
            Render(render, static_cast<const bugattack::tower::Laser&>(tower));
            break;
        case bugattack::UnitType::RocketGun:
            Render(render, static_cast<const bugattack::tower::RocketGun&>(tower));
            break;
        default:
            assert(false && "Invalid tower type");
    }
}

void Render(class Render& render, const bugattack::Map& map)
{
    render.DrawUnit(map);

    std::for_each(map.begin_enemy(), map.end_enemy(), [&render](auto&& enemy){
        render.DrawUnit(enemy);
    });

    std::for_each(map.begin_tower(), map.end_tower(), [&render](auto&& tower){
        Render(render, tower);
    });

    render.DrawText(utils::Format("Level: %2d", 9, map.Level()), 640.0f, 440.0f, {0.5, 0.8f, 0.2f, 1.0f});

    render.DrawText(utils::Format("Health: %2d", 10, map.Health()), 640.0f, 410.0f, {0.5, 0.8f, 0.2f, 1.0f});

    render.DrawText(utils::Format("Gold: %4d", 10, map.Gold()), 640.0f, 380.0f, {0.5, 0.8f, 0.2f, 1.0f});

    render.DrawText(utils::Format("Wave: %5.2f", 11, map.TimeOffset()), 640.0f, 350.0f, {0.5, 0.8f, 0.2f, 1.0f});
}

}
