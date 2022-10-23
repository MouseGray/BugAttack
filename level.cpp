#include "level.h"

#include <fstream>
#include <charconv>
#include <numeric>
#include <iostream>
#include <utility>

namespace bugattack {

Level::Level(const std::string &path) :
    time_offset_{}, spawn_offset_{},
    enemy_count_{std::numeric_limits<decltype(enemy_count_)>::max()},
    generate_type_{-1},
    current_level_{}
{
    std::ifstream in{ path };

    if(!in.is_open())
        throw std::runtime_error{ "Failed to open " + path };

    std::vector<LevelConfiguration> new_levels;

    for(std::string line; std::getline(in, line);)
    {
        LevelConfiguration configuration;

        if(line.empty())
            continue;

        std::size_t idx = 0;
        configuration.time_offset_ = std::stof(line, &idx);
        ++idx;

        auto type_by_id = [](int val) -> UnitType {
            switch(val)
            {
                case 1: return UnitType::Bug1;
                case 2: return UnitType::Bug2;
                case 3: return UnitType::Bug3;
            }
            assert(false && "Invalid enemy type");
        };

        for(; idx < line.size(); ++idx)
        {
            auto idx_ = idx;
            configuration.enemies_.push_back(type_by_id(std::stoi(line.substr(idx), &idx_)));
            idx += idx_;
        }

        new_levels.push_back(std::move(configuration));
    }

    levels_ = std::move(new_levels);
    time_offset_ = levels_[current_level_].time_offset_;
}

void Level::Update(float time) noexcept
{
    if(IsFinished())
        return;

    if(enemy_count_ < levels_[current_level_ - 1].enemies_.size())
    {
        spawn_offset_ -= time;

        if(spawn_offset_ < 0.0f)
        {
            generate_type_ = levels_[current_level_ - 1].enemies_[enemy_count_];
            spawn_offset_ = MAX_SPAWN_OFFSET;
            ++enemy_count_;
        }
    }

    time_offset_ -= time;
    if(time_offset_ <= 0.0f)
    {
        ++current_level_;
        if(current_level_ == levels_.size())
        {
            time_offset_ = 0.0f;
        }
        else
        {
            enemy_count_ = 0;
            spawn_offset_ = MAX_SPAWN_OFFSET;
            time_offset_ = levels_[current_level_].time_offset_;
        }
    }
}

bool Level::IsNeedGenerate() const noexcept
{
    return static_cast<int>(generate_type_) != -1;
}

UnitType Level::EnemyType() noexcept
{
    assert(IsNeedGenerate() && "Generation is not required");

    return std::exchange(generate_type_, static_cast<UnitType>(-1));
}

std::size_t Level::CurrentLevel() const noexcept
{
    return current_level_;
}

float Level::TimeOffset() const noexcept
{
    return time_offset_;
}

bool Level::IsFinished() const noexcept
{
    return current_level_ == levels_.size();
}

void Level::Restart() noexcept
{
    current_level_ = 0;
    time_offset_ = levels_[current_level_].time_offset_;
    spawn_offset_ = 0.0f;
    enemy_count_ = std::numeric_limits<decltype(enemy_count_)>::max();
    generate_type_ = static_cast<UnitType>(-1);
}

}
