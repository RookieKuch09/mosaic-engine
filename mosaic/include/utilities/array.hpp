#pragma once

#include <cstdint>
#include <type_traits>

namespace Mosaic
{
    template <typename T, std::uint64_t Size>
    class FixedArray
    {
    public:
        FixedArray();

        template <typename... U, typename = std::enable_if_t<(sizeof...(U) == Size) and (std::conjunction_v<std::is_convertible<U, T>...>)>>
        constexpr FixedArray(U&&... args);

        FixedArray(const FixedArray<T, Size>& other);
        FixedArray(FixedArray<T, Size>&& other) noexcept;

    private:
        T mElements[Size];
    };

    template <typename T>
    class DynamicArray
    {
    public:
    private:
    };
}