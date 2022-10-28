#include "map.h"

#include <algorithm>
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
        enemy::Update(*enemy, field_, time);

    for(auto&& tower: towers_)
        tower->Update(enemies_, time);

    health_ -= enemy::InEnd(enemies_.begin(), enemies_.end(), field_);

    gold_ += enemy::Cost(enemies_.begin(), enemies_.end());

    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [this](auto&& enemy){
        return enemy->IsDestroyed() || enemy::InEnd(field_, *enemy);
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

    level_.Restart();
}

const std::vector<std::shared_ptr<enemy::Enemy>>& Map::Enemies() const noexcept
{
    return enemies_;
}

const std::vector<std::unique_ptr<tower::TowerBase>>& Map::Towers() const noexcept
{
    return towers_;
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

std::shared_ptr<enemy::Enemy> CreateEnemy(UnitType type, std::size_t x, std::size_t y, int level)
{
    switch (type)
    {
        case UnitType::Bug1:
            return std::make_shared<enemy::Ant>(::Geometry{{ x*20, y*20 }, 90.0f}, level);
        case UnitType::Bug2:
            return std::make_shared<enemy::Ladybug>(::Geometry{{ x*20, y*20 }, 90.0f }, level);
        case UnitType::Bug3:
            return std::make_shared<enemy::Cockroach>(::Geometry{{ x*20, y*20 }, 90.0f }, level);
        default:
            assert(false && "Invalid enemy type");
    }
}

std::unique_ptr<tower::TowerBase> CreateTower(UnitType type, std::size_t x, std::size_t y)
{
    switch (type)
    {
        case UnitType::Gun:
            return std::make_unique<tower::Gun>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f});
        case UnitType::Laser:
            return std::make_unique<tower::Laser>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f});
        case UnitType::RocketGun:
            return std::make_unique<tower::RocketGun>(::Geometry{{ x*20.0f, y*20.0f }, 270.0f});
        default:
            assert(false && "Invalid tower type");
    }
}

}
