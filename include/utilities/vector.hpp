#pragma once

#include "utilities/typeinfo.hpp"

namespace Mosaic::Internal::Types
{

    template <TypeConcepts::Numeric T, UI32 Length>
    class Vector
    {
    public:
        using ValueType = T;
    };

    template <TypeConcepts::Numeric T>
    class Vector<T, 2>
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

        Vector();

        Vector(T all);
        Vector(T x, T y);

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector<T, 2>& operator=(const Vector& other);
        Vector<T, 2>& operator=(Vector&& other) noexcept;
    };

    template <TypeConcepts::Numeric T>
    class Vector<T, 3>
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

        Vector();

        Vector(T all);
        Vector(T x, T y, T z);

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector<T, 3>& operator=(const Vector& other);
        Vector<T, 3>& operator=(Vector&& other) noexcept;
    };

    template <TypeConcepts::Numeric T>
    class Vector<T, 4>
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

        Vector();

        Vector(T all);
        Vector(T x, T y, T z, T w);

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector<T, 4>& operator=(const Vector& other);
        Vector<T, 4>& operator=(Vector&& other) noexcept;
    };

    template <typename T>
    using Vector2 = Vector<T, 2>;

    template <typename T>
    using Vector3 = Vector<T, 3>;

    template <typename T>
    using Vector4 = Vector<T, 4>;
}

#include "utilities/vector.inl"