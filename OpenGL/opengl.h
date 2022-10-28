#pragma once

#define USE_IMPL(type, line, obj) opengl::Use ## type $ ## line {obj}
#define USE_LINE(type, line, obj) USE_IMPL(type, line, obj)
#define USE(type, obj) USE_LINE(type, __LINE__, obj)

struct Using
{
    Using() = default;

    Using(const Using&) = delete;
    Using(Using&&) = delete;

    Using& operator=(const Using&) = delete;
    Using& operator=(Using&&) = delete;

    virtual ~Using() = 0;
};

inline Using::~Using() {}
