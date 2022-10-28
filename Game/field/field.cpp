#include "field.h"

#include <algorithm>
#include <fstream>

namespace bugattack::field {

CellType Field::ConstRow::operator[](std::size_t idx) const noexcept
{
    assert(idx < size_ && "Out of range");

    return data_[idx];
}

CellType& Field::Row::operator[](std::size_t idx) noexcept
{
    assert(idx < size_ && "Out of range");

    return data_[idx];
}

Field::Field(const std::string &path)
{
    std::ifstream in{ path };

    if(!in.is_open())
        throw std::runtime_error{ "Failed to open " + path };

    std::size_t new_width{};
    std::size_t new_height{};
    std::vector<CellType> new_data;

    for(std::string line; std::getline(in, line); ++new_height)
    {
        if(line.empty())
            continue;

        if(!new_width)
        {
            new_width = line.size();
        }
        else if(new_width != line.size())
        {
            throw std::runtime_error{ "Invalid row length" };
        }
        std::transform(line.begin(), line.end(), std::back_inserter(new_data), [](auto&& val)
        {
            return static_cast<CellType>(val);
        });
    }

    auto spawn_it = std::find(new_data.begin(), new_data.end(), CellType::Spawn);

    if(spawn_it == new_data.end())
        throw std::runtime_error{ "Spawn point not set" };

    auto end_it = std::find(new_data.begin(), new_data.end(), CellType::End);

    if(end_it == new_data.end())
        throw std::runtime_error{ "End point not set" };

    width_ = new_width;
    height_ = new_height;

    spawn_ = std::distance(new_data.begin(), spawn_it);
    end_ = std::distance(new_data.begin(), end_it);

    data_ = std::move(new_data);
}

Field::ConstRow Field::operator[](std::size_t idx) const noexcept
{
    assert(idx < height_ && "Out of range");
#ifndef NDEBUG
    return { data_.data() + idx*width_, width_ };
#else
    return { data_.data() + idx*width_ };
#endif
}

Field::Row Field::operator[](std::size_t idx) noexcept
{
    assert(idx < height_ && "Out of range");
#ifndef NDEBUG
    return { data_.data() + idx*width_, width_ };
#else
    return { data_.data() + idx*width_ };
#endif
}

std::pair<std::size_t, std::size_t> Field::SpawnPoint() const noexcept
{
    return { spawn_ / width_, spawn_ % width_ };
}

std::pair<std::size_t, std::size_t> Field::EndPoint() const noexcept
{
    return { end_ / width_, end_ % width_ };
}

bool IsFree(const Field& field, std::size_t x, std::size_t y) noexcept
{
    return field[y][x] != CellType::Tower && !IsPath(field, x, y);
}

bool IsFree(const Field& field, std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1) noexcept
{
    assert(x0 <= x1 && y0 <= y1 && "Invalid range");

    if(field.Width() < x1 || field.Height() < y1)
        return false;

    for(auto it_y = y0, end_y = y1 + 1; it_y != end_y; ++it_y)
    {
        for(auto it_x = x0, end_x = x1 + 1; it_x != end_x; ++it_x)
        {
            if(!IsFree(field, it_x, it_y))
            {
                return false;
            }
        }
    }
    return true;
}

bool IsPath(const Field &field, std::size_t x, std::size_t y)
{
    return field[y][x] == CellType::Spawn || field[y][x] == CellType::Path || field[y][x] == CellType::End;
}

}
