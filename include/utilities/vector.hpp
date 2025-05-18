#pragma once

#include "utilities/arithmetic.hpp"

#include <type_traits>

namespace Mosaic
{
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <typename T>
    struct IsVector : std::false_type
    {
    };

    template <Arithmetic T, uint32 Length>
    class Vector
    {
    public:
        using ValueType = T;
    };

    template <Arithmetic T, uint32 N>
    struct IsVector<Vector<T, N>> : std::true_type
    {
    };

    template <typename T>
    constexpr bool IsVectorV = IsVector<T>::value;

    template <Arithmetic T>
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

    template <Arithmetic T>
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

    template <Arithmetic T>
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
    struct VectorType
    {
        using Type = T;
    };

    template <Arithmetic T, uint32 N>
    struct VectorType<Vector<T, N>>
    {
        using Type = T;
    };

    template <typename T>
    using VectorTypeT = typename VectorType<T>::Type;

    template <typename T>
    struct VectorElementCount : std::integral_constant<uint32, 1>
    {
    };

    template <typename T>
    struct VectorElementCount<Vector<T, 2>> : std::integral_constant<uint32, 2>
    {
    };

    template <typename T>
    struct VectorElementCount<Vector<T, 3>> : std::integral_constant<uint32, 3>
    {
    };

    template <typename T>
    struct VectorElementCount<Vector<T, 4>> : std::integral_constant<uint32, 4>
    {
    };

    template <typename T>
    constexpr uint32 VectorElementCountV = VectorElementCount<T>::value;

    template <typename T>
    using Vector2 = Vector<T, 2>;

    template <typename T>
    using Vector3 = Vector<T, 3>;

    template <typename T>
    using Vector4 = Vector<T, 4>;
}

#include "utilities/vector.inl"