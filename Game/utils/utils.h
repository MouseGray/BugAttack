#pragma once

#include <string>

#include <glm/glm.hpp>

#include "primitives.h"

namespace utils
{

PURE glm::vec2 Direction(float angle) noexcept;

PURE glm::vec2 Direction(glm::vec2 from, glm::vec2 to) noexcept;

PURE float Angle(glm::vec2 direction, glm::vec2 vector_null = {1.0f, 0.0f}) noexcept;

PURE bool IsEqual(glm::vec2 lhs, glm::vec2 rhs) noexcept;

PURE bool IsCodirectional(glm::vec2 lhs, glm::vec2 rhs) noexcept;

PURE bool InRange(glm::vec2 obj, glm::vec2 point, float radius) noexcept;

template<typename ...Args>
std::string Format(std::string_view format, std::string::size_type length, Args&& ...args)
{
    std::string result(length, '\0');
    sprintf(result.data(), format.data(), std::forward<Args>(args)...);
    return result;
}

template<typename It>
struct DereferenceIterator
{
    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::iterator_traits<It>::value_type::element_type;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = typename std::iterator_traits<It>::difference_type;

    DereferenceIterator(It it) :
        it_{std::move(it)}
    {

    }

    bool operator==(const DereferenceIterator<It>& it) const noexcept
    {
        return it_ == it.it_;
    }

    bool operator!=(const DereferenceIterator<It>& it) const noexcept
    {
        return it_ != it.it_;
    }

    reference operator*()
    {
        return **it_;
    }

    pointer operator->()
    {
        return std::addressof(**it_);
    }

    DereferenceIterator<It>& operator++()
    {
        ++it_;
        return *this;
    }

    DereferenceIterator<It> operator++(int)
    {
        DereferenceIterator<It> tmp{*this};
        ++(*this);
        return tmp;
    }

    It it_;
};

}
