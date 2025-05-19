#pragma once

#include "utilities/vector.hpp"

namespace Mosaic::Internal::Types
{
    template <TypeConcepts::Numeric T>
    Vector<T, 2>::Vector()
        : X(0), Y(0)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>::Vector(T all)
        : X(all), Y(all)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>::Vector(T x, T y)
        : X(x), Y(y)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>::Vector(const Vector& other)
        : X(other.X), Y(other.Y)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>::Vector(Vector&& other) noexcept
        : X(other.X), Y(other.Y)
    {
        other.X = 0;
        other.Y = 0;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>& Vector<T, 2>::operator=(const Vector& other)
    {
        X = other.X;
        Y = other.Y;

        return *this;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 2>& Vector<T, 2>::operator=(Vector&& other) noexcept
    {
        X = other.X;
        Y = other.Y;

        other.X = 0;
        other.Y = 0;

        return *this;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>::Vector()
        : X(0), Y(0), Z(0)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>::Vector(T all)
        : X(all), Y(all), Z(all)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>::Vector(T x, T y, T z)
        : X(x), Y(y), Z(z)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>::Vector(const Vector& other)
        : X(other.X), Y(other.Y), Z(other.Z)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>::Vector(Vector&& other) noexcept
        : X(other.X), Y(other.Y), Z(other.Z)
    {
        other.X = 0;
        other.Y = 0;
        other.Z = 0;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>& Vector<T, 3>::operator=(const Vector& other)
    {
        X = other.X;
        Y = other.Y;
        Z = other.Z;

        return *this;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 3>& Vector<T, 3>::operator=(Vector&& other) noexcept
    {
        X = other.X;
        Y = other.Y;
        Z = other.Z;

        other.X = 0;
        other.Y = 0;
        other.Z = 0;

        return *this;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>::Vector()
        : X(0), Y(0), Z(0), W(0)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>::Vector(T all)
        : X(all), Y(all), Z(all), W(all)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>::Vector(T x, T y, T z, T w)
        : X(x), Y(y), Z(z), W(w)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>::Vector(const Vector& other)
        : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
    {
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>::Vector(Vector&& other) noexcept
        : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
    {
        other.X = 0;
        other.Y = 0;
        other.Z = 0;
        other.W = 0;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>& Vector<T, 4>::operator=(const Vector& other)
    {
        X = other.X;
        Y = other.Y;
        Z = other.Z;
        W = other.W;

        return *this;
    }

    template <TypeConcepts::Numeric T>
    Vector<T, 4>& Vector<T, 4>::operator=(Vector&& other) noexcept
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
}