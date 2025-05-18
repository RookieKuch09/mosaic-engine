#pragma once

#include "utilities/vector.hpp"

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>::Vector()
    : X(0), Y(0)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>::Vector(T all)
    : X(all), Y(all)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>::Vector(T x, T y)
    : X(x), Y(y)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>::Vector(const Vector& other)
    : X(other.X), Y(other.Y)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>::Vector(Vector&& other) noexcept
    : X(other.X), Y(other.Y)
{
    other.X = 0;
    other.Y = 0;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>& Mosaic::Vector<T, 2>::operator=(const Vector& other)
{
    X = other.X;
    Y = other.Y;

    return *this;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 2>& Mosaic::Vector<T, 2>::operator=(Vector&& other) noexcept
{
    X = other.X;
    Y = other.Y;

    other.X = 0;
    other.Y = 0;

    return *this;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>::Vector()
    : X(0), Y(0), Z(0)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>::Vector(T all)
    : X(all), Y(all), Z(all)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>::Vector(T x, T y, T z)
    : X(x), Y(y), Z(z)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>::Vector(const Vector& other)
    : X(other.X), Y(other.Y), Z(other.Z)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>::Vector(Vector&& other) noexcept
    : X(other.X), Y(other.Y), Z(other.Z)
{
    other.X = 0;
    other.Y = 0;
    other.Z = 0;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>& Mosaic::Vector<T, 3>::operator=(const Vector& other)
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;

    return *this;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 3>& Mosaic::Vector<T, 3>::operator=(Vector&& other) noexcept
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;

    other.X = 0;
    other.Y = 0;
    other.Z = 0;

    return *this;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>::Vector()
    : X(0), Y(0), Z(0), W(0)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>::Vector(T all)
    : X(all), Y(all), Z(all), W(all)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>::Vector(T x, T y, T z, T w)
    : X(x), Y(y), Z(z), W(w)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>::Vector(const Vector& other)
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>::Vector(Vector&& other) noexcept
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
    other.X = 0;
    other.Y = 0;
    other.Z = 0;
    other.W = 0;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>& Mosaic::Vector<T, 4>::operator=(const Vector& other)
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;
    W = other.W;

    return *this;
}

template <Mosaic::Arithmetic T>
Mosaic::Vector<T, 4>& Mosaic::Vector<T, 4>::operator=(Vector&& other) noexcept
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