#include "map.h"

#include <fstream>

#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>
#include <tower/gun.h>
#include <tower/laser.h>
#include <tower/rocket.h>

#include "level.h"

Map::Map() :
    Unit{UnitType::Map, {{310.0f, 310.0f}, 270.0f}},
    level_("levels/default.lvl")
{
    health_ = 3;
    gold_ = 500;
    field_.Load("maps/default.map");
}

void Map::Update(float time)
{
    level_.Update(time);

    if(level_.IsNeedGenerate())
        GenerateEnemy(level_.EnemyType());

    for(auto&& a: enemies_)
    {
        a->Move(field_, time);
    }

    for(auto&& a: towers_)
    {
        a->Update(enemies_, time);
    }

    auto in_end = std::count_if(enemies_.begin(), enemies_.end(), [](const auto& r)
    {
        return r->InEnd();
    });

    health_ -= in_end;

    gold_ = std::accumulate(enemies_.begin(), enemies_.end(), gold_, [](int sum, const auto& e)
    {
        if(e->IsLive())
            return sum;
        return sum + e->Cost();
    });

    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const auto& r) mutable
    {
        return (!r->IsLive() || r->InEnd());
    }), enemies_.end());
}

void Map::GenerateEnemy(UnitType t)
{
    auto [spawn_y, spawn_x] = field_.SpawnPoint();

    auto* enemy = new Enemy{t, ::Geometry{ { spawn_x*20, spawn_y*20 }, 90.0f }, static_cast<int>(level_.CurrentLevel())};

    enemies_.push_back(std::shared_ptr<Enemy>(enemy));
}

bool Map::CanPut(const glm::vec2& pos, const glm::vec2& size) const noexcept
{
    auto p_pos = pos/20.0f;

    if(p_pos.x < 1 || p_pos.x >= field_.Width() - 1)
        return false;

    if(p_pos.y < 1 || p_pos.y >= field_.Height() - 1)
        return false;

    return field_.CanPut(p_pos.x, p_pos.y);
}

void Map::Put(UnitType type, size_t x, size_t y) noexcept
{
    if(!field_.CanPut(x, y))
        return;

    switch (type) {
        case UnitType::Gun:
            if(gold_ < tower::Gun::COST)
                return;
            gold_ -= tower::Gun::COST;
            towers_.push_back(std::make_unique<tower::Gun>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f}));
            break;
        case UnitType::Laser:
            if(gold_ < tower::Laser::COST)
                return;
            gold_ -= tower::Laser::COST;
            towers_.push_back(std::make_unique<tower::Laser>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f}));
            break;
        case UnitType::RocketGun:
            if(gold_ < tower::RocketGun::COST)
                return;
            gold_ -= tower::RocketGun::COST;
            towers_.push_back(std::make_unique<tower::RocketGun>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f}));
            break;
    }

    field_.Put(x, y);
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
    towers_.clear();
    enemies_.clear();

    level_.Restart();
    health_ = 3;
    gold_ = 500;
}

const std::vector<std::shared_ptr<Enemy>>& Map::Enemies() const noexcept
{
    return enemies_;
}

const std::vector<std::unique_ptr<tower::TowerBase>>& Map::Towers() const noexcept
{
    return towers_;
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
