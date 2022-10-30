#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include <field/field.h>
#include <utils/utils.h>

#include "game_enemy.h"
#include "game_tower.h"
#include "level.h"

namespace bugattack
{

class Map : public Unit
{
    using EnemyIterator = utils::DereferenceIteratorConst<std::vector<GameEnemy>::const_iterator>;
    using TowerIterator = utils::DereferenceIteratorConst<std::vector<GameTower>::const_iterator>;

    static constexpr int MAX_HEALTH = 3;
    static constexpr int START_GOLD = 2000;
public:
    Map();

    void Update(float time);

    bool CanPut(const glm::vec2& pos) const noexcept;

    void Put(UnitType type, size_t x, size_t y) noexcept;

    void Restart();

    EnemyIterator begin_enemy() const noexcept;

    EnemyIterator end_enemy() const noexcept;

    TowerIterator begin_tower() const noexcept;

    TowerIterator end_tower() const noexcept;

    bool IsWin() const noexcept;

    bool IsLost() const noexcept;

    int Gold() const noexcept;

    int Health() const noexcept;

    int Level() const noexcept;

    float TimeOffset() const noexcept;
public:
    void GenerateEnemy(UnitType type);

    int gold_;
    int health_;

    std::vector<GameEnemy> enemies_;

    std::vector<GameTower> towers_;

    class Level level_;

    field::Field field_;
};

GameEnemy CreateEnemy(UnitType type, std::size_t x, std::size_t y, int level);

GameTower CreateTower(UnitType type, std::size_t x, std::size_t y);

}

