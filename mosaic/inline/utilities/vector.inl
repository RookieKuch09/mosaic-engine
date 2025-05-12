#pragma once

#include "../../include/utilities/vector.hpp"

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>::Vector2()
    : X(0), Y(0)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>::Vector2(T all)
    : X(all), Y(all)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>::Vector2(T x, T y)
    : X(x), Y(y)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>::Vector2(const Vector2& other)
    : X(other.X), Y(other.Y)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>::Vector2(Vector2&& other) noexcept
    : X(other.X), Y(other.Y)
{
    other.X = 0;
    other.Y = 0;
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>& Mosaic::Vector2<T>::operator=(const Vector2& other)
{
    X = other.X;
    Y = other.Y;

    return *this;
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector2<T>& Mosaic::Vector2<T>::operator=(Vector2&& other) noexcept
{
    X = other.X;
    Y = other.Y;

    other.X = 0;
    other.Y = 0;

    return *this;
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>::Vector4()
    : X(0), Y(0), Z(0), W(0)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>::Vector4(T all)
    : X(all), Y(all), Z(all), W(all)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>::Vector4(T x, T y, T z, T w)
    : X(x), Y(y), Z(z), W(w)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>::Vector4(const Vector4& other)
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>::Vector4(Vector4&& other) noexcept
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
    other.X = 0;
    other.Y = 0;
    other.Z = 0;
    other.W = 0;
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>& Mosaic::Vector4<T>::operator=(const Vector4& other)
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;
    W = other.W;

    return *this;
}

template <typename T> requires std::is_arithmetic_v<T>
Mosaic::Vector4<T>& Mosaic::Vector4<T>::operator=(Vector4&& other) noexcept
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;
    W = other.W;

    other.X = 0;
    other.Y = 0;
    other.Z = 0;
    other.W = 0;

    return *this;
}