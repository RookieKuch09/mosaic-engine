#pragma once

#include "utilities/typeinfo.hpp"

namespace Mosaic::Internal::Types
{

    template <TypeConcepts::Numeric T, UI32 Length>
    class Vec
    {
    public:
        using ValueType = T;
    };

    template <TypeConcepts::Numeric T>
    class Vec<T, 2>
    {
    public:
        using ValueType = T;

        union
        {
            struct
            {
                T X, Y;
            };

            T Elements[2];
        };

        Vec();

        Vec(T all);
        Vec(T x, T y);

        Vec(const Vec& other);
        Vec(Vec&& other) noexcept;

        Vec<T, 2>& operator=(const Vec& other);
        Vec<T, 2>& operator=(Vec&& other) noexcept;
    };

    template <TypeConcepts::Numeric T>
    class Vec<T, 3>
    {
    public:
        using ValueType = T;

        union
        {
            struct
            {
                T X, Y, Z;
            };

            T Elements[3];
        };

        Vec();

        Vec(T all);
        Vec(T x, T y, T z);

        Vec(const Vec& other);
        Vec(Vec&& other) noexcept;

        Vec<T, 3>& operator=(const Vec& other);
        Vec<T, 3>& operator=(Vec&& other) noexcept;
    };

    template <TypeConcepts::Numeric T>
    class Vec<T, 4>
    {
    public:
        using ValueType = T;

        union
        {
            struct
            {
                T X, Y, Z, W;
            };

            T Elements[4];
        };

        Vec();

        Vec(T all);
        Vec(T x, T y, T z, T w);

        Vec(const Vec& other);
        Vec(Vec&& other) noexcept;

        Vec<T, 4>& operator=(const Vec& other);
        Vec<T, 4>& operator=(Vec&& other) noexcept;
    };

    template <typename T>
    using Vec2 = Vec<T, 2>;

    template <typename T>
    using Vec3 = Vec<T, 3>;

    template <typename T>
    using Vec4 = Vec<T, 4>;
}

#include "utilities/vector.inl"