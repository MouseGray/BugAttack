#include "map.h"

#include <fstream>

#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>

#include "level.h"
#include "render.h"

Map::Map(GLuint texture_id, GLfloat x, GLfloat y, GLfloat w, GLfloat h) :
    texture_id_{texture_id}, x_{x}, y_{y}, w_{w}, h_{h},
    level_("levels/default.lvl")
{
    health_ = 3;
    gold_ = 500;
    field_.Load( "maps/default.map" );
}

void Map::Update(float time)
{
    level_.Update(time);

    if(level_.IsNeedGenerate())
        GenerateEnemy(level_.EnemyType());

    for(auto&& a: enemies_)
    {
        a.Move(field_, time);
    }

    for(auto&& a: towers_)
    {
        a.Attack(enemies_.begin(), enemies_.end(), time);
    }

    auto in_end = std::count_if(enemies_.begin(), enemies_.end(), [](const auto& r)
    {
        return r.InEnd();
    });

    health_ -= in_end;

    gold_ = std::accumulate(enemies_.begin(), enemies_.end(), gold_, [](int sum, const Enemy& e)
    {
        if(e.IsLive())
            return sum;
        return sum + e.Cost();
    });

    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const auto& r)
    {
        return !r.IsLive() || r.InEnd();
    }), enemies_.end());
}

void Map::GenerateEnemy(UnitType t)
{
    auto [spawn_y, spawn_x] = field_.SpawnPoint();

    enemies_.push_back(Enemy{ t, Point{ spawn_x*20, spawn_y*20 }, 270.0f, 0 });
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

    field_.Put(x, y);

    towers_.push_back({ type, glm::vec2{ x*20.0f, y*20.0f }, 90.0f });
}

void Map::Render(class Render& render)
{
    auto std_position = glm::vec2{x_, y_} + 0.5f*glm::vec2{w_, h_};
    render.DrawTexture(texture_id_, std_position.x, std_position.y, w_, h_);

    for(auto&& enemy: enemies_)
    {
        render.DrawUnit(enemy);
    }

    for(auto&& tower: towers_)
    {
        tower.Render(render);
    }

    std::string level(9, '\0');
    sprintf(level.data(), "Level: %2lu", level_.CurrentLevel());
    render.DrawText(level, 640.0f, 440.0f, {0.5, 0.8f, 0.2f, 1.0f});


    std::string Health(10, '\0');
    sprintf(Health.data(), "Health: %2d", health_);
    render.DrawText(Health, 640.0f, 410.0f, {0.5, 0.8f, 0.2f, 1.0f});


    std::string Gold(10, '\0');
    sprintf(Gold.data(), "Gold: %4d", gold_);
    render.DrawText(Gold, 640.0f, 380.0f, {0.5, 0.8f, 0.2f, 1.0f});

    std::string text(11, '\0');
    sprintf(text.data(), "Wave: %5.2f", level_.TimeOffset());
    render.DrawText(text, 640.0f, 350.0f, {0.5, 0.8f, 0.2f, 1.0f});
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
