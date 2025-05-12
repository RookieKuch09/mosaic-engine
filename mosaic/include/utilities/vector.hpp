#pragma once

#include <type_traits>

namespace Mosaic
{
    template <typename T> requires std::is_arithmetic_v<T>
    class Vector2
    {
    public:
        union
        {
            struct
            {
                T X, Y;
            };

            T Elements[4];
        };

        Vector2();

        Vector2(T all);
        Vector2(T x, T y);

        Vector2(const Vector2& other);
        Vector2(Vector2&& other) noexcept;

        Vector2<T>& operator=(const Vector2& other);
        Vector2<T>& operator=(Vector2&& other) noexcept;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    class Vector4
    {
    public:
        union
        {
            struct
            {
                T X, Y, Z, W;
            };

            T Elements[4];
        };

        Vector4();

        Vector4(T all);
        Vector4(T x, T y, T z, T w);

        Vector4(const Vector4& other);
        Vector4(Vector4&& other) noexcept;

        Vector4<T>& operator=(const Vector4& other);
        Vector4<T>& operator=(Vector4&& other) noexcept;
    };
}

#include "../../inline/utilities/vector.inl"