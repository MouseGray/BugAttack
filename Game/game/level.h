#pragma once

#include <array>
#include <string>
#include <vector>

#include <unit/unit.h>

namespace bugattack
{

class Level
{
    static constexpr float MAX_SPAWN_OFFSET = 0.7f;

    struct LevelConfiguration
    {
        float time_offset_;
        std::vector<UnitType> enemies_;
    };
public:
    Level(const std::string& path);

    void Update(float time) noexcept;

    bool IsNeedGenerate() const noexcept;

    UnitType EnemyType() noexcept;

    std::size_t CurrentLevel() const noexcept;

    float TimeOffset() const noexcept;

    bool IsFinished() const noexcept;

    void Restart() noexcept;
private:

    float time_offset_;
    float spawn_offset_;
    std::size_t enemy_count_;
    UnitType generate_type_;

    std::size_t current_level_;

    std::vector<LevelConfiguration> levels_;
};

}

