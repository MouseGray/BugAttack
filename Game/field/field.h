#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace bugattack::field
{

enum class CellType : unsigned char
{
    Spawn = 'S',
    End = 'E',
    Path = 'P',
    Wall = '1',
    Field = '0',
    Tower = 'T'
};

class Field
{
    class ConstRow
    {
    public:
#ifndef NDEBUG
        ConstRow(const CellType* data, std::size_t size) :
            data_{data}, size_{size}
        {};
#else
        ConstRow(cell_t* data) :
            data_{data}
        {};
#endif

        CellType operator[](std::size_t idx) const noexcept;

    private:
        const CellType* data_;
#ifndef NDEBUG
        std::size_t size_;
#endif
    };
    class Row
    {
    public:
#ifndef NDEBUG
        Row(CellType* data, std::size_t size) :
            data_{data}, size_{size}
        {};
#else
        Row(cell_t* data) :
            data_{data}
        {};
#endif

        CellType& operator[](std::size_t idx) noexcept;

    private:
        CellType* data_;
#ifndef NDEBUG
        std::size_t size_;
#endif
    };
public:
    Field(const std::string& path);

    ConstRow operator[](std::size_t idx) const noexcept;

    Row operator[](std::size_t idx) noexcept;

    inline std::size_t Width() const noexcept { return width_; }
    inline std::size_t Height() const noexcept { return height_; }

    std::pair<std::size_t, std::size_t> SpawnPoint() const noexcept;
    std::pair<std::size_t, std::size_t> EndPoint() const noexcept;
private:
    std::size_t width_;
    std::size_t height_;

    std::size_t spawn_;
    std::size_t end_;

    std::vector<CellType> data_;
};

bool IsFree(const Field& field, std::size_t x, std::size_t y) noexcept;

bool IsFree(const Field& field, std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1) noexcept;

bool IsPath(const Field& field, std::size_t x, std::size_t y);

}
