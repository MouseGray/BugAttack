#include "map.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <memory>
#include <iterator>

#include <tower/gun.h>
#include <tower/laser.h>
#include <tower/rocket.h>

#include <enemy/ant.h>
#include <enemy/cockroach.h>
#include <enemy/ladybug.h>

#include "level.h"

namespace bugattack
{

Map::Map() :
    Unit{UnitType::Map, {{310.0f, 310.0f}, 270.0f}},
    level_{"../levels/default.lvl"}, field_{"../maps/default.map"}
{
    Restart();
}

void Map::Update(float time)
{
    level_.Update(time);

    if(level_.IsNeedGenerate())
        GenerateEnemy(level_.EnemyType());

    for(auto&& enemy: enemies_)
        bugattack::Update(enemy, field_, time);

    for(auto&& tower: towers_)
        bugattack::Update(tower, begin_enemy(), end_enemy(), time);

    for(auto&& enemy: enemies_)
    {
        health_ -= bugattack::InEnd(enemy, field_);
        gold_   += bugattack::Cost(enemy);
    }

    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [this](auto&& enemy){
        return enemy->IsDestroyed() || bugattack::InEnd(enemy, field_);
    }), enemies_.end());
}

void Map::GenerateEnemy(UnitType type)
{
    auto [spawn_y, spawn_x] = field_.SpawnPoint();

    enemies_.push_back(CreateEnemy(type, spawn_x, spawn_y, Level()));
}

bool Map::CanPut(const glm::vec2& pos) const noexcept
{
    auto p_pos = pos;

    return IsFree(field_, p_pos.x - 1, p_pos.y - 1, p_pos.x + 1, p_pos.y + 1);
}

void Map::Put(UnitType type, size_t x, size_t y) noexcept
{
    if(!CanPut(glm::vec2{x, y}))
        return;

    auto cost = tower::Cost(type);

    if(gold_ < cost)
        return;
    gold_ -= cost;

    towers_.push_back(CreateTower(type, x, y));

    field_[y][x] = field::CellType::Tower;
}

bool Map::IsWin() const noexcept
{
    return level_.IsFinished() && enemies_.empty();
}

bool Map::IsLost() const noexcept
{
    return health_ < 0;
}

void Map::Restart()
{
    health_ = MAX_HEALTH;
    gold_ = START_GOLD;

    towers_.clear();
    enemies_.clear();

    field_ = field::Field{"../maps/default.map"};

    level_.Restart();
}

Map::EnemyIterator Map::begin_enemy() const noexcept
{
    return {enemies_.begin()};
}

Map::EnemyIterator Map::end_enemy() const noexcept
{
    return {enemies_.end()};
}

Map::TowerIterator Map::begin_tower() const noexcept
{
    return {towers_.begin()};
}

Map::TowerIterator Map::end_tower() const noexcept
{
    return {towers_.end()};
}

int Map::Gold() const noexcept
{
    return gold_;
}

int Map::Health() const noexcept
{
    return health_;
}

int Map::Level() const noexcept
{
    return level_.CurrentLevel();
}

float Map::TimeOffset() const noexcept
{
    return level_.TimeOffset();
}

GameEnemy CreateEnemy(UnitType type, std::size_t x, std::size_t y, int level)
{
    switch (type)
    {
        case UnitType::Bug1:
            return enemy::Ant{::Geometry{{ x*20, y*20 }, 90.0f}, level};
        case UnitType::Bug2:
            return enemy::Ladybug{::Geometry{{ x*20, y*20 }, 90.0f }, level};
        case UnitType::Bug3:
            return enemy::Cockroach{::Geometry{{ x*20, y*20 }, 90.0f }, level};
        default:
            assert(false && "Invalid enemy type");
    }
}

GameTower CreateTower(UnitType type, std::size_t x, std::size_t y)
{
    switch (type)
    {
        case UnitType::Gun:
            return tower::Gun{::Geometry{{ x*20.0f, y*20.0f }, 270.0f}};
        case UnitType::Laser:
            return tower::Laser{::Geometry{{ x*20.0f, y*20.0f }, 270.0f}};
        case UnitType::RocketGun:
            return tower::RocketGun{::Geometry{{ x*20.0f, y*20.0f }, 270.0f}};
        default:
            assert(false && "Invalid tower type");
    }
}

}
