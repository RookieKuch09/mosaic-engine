#pragma once

#include "utilities/numerics.hpp"

namespace Mosaic::Internal::TypeChecks
{
    template <typename T>
    struct IsNumeric
    {
        static constexpr bool Result = false;
    };

    template <>
    struct IsNumeric<Types::Float32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::Float64>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::Int8>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::Int16>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::Int32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::Int64>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UInt8>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UInt16>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UInt32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UInt64>
    {
        static constexpr bool Result = true;
    };

    template <typename T>
    constexpr bool IsNumericV = IsNumeric<T>::Result;
}

namespace Mosaic::Internal::TypeConcepts
{
    template <typename T>
    concept Numeric = TypeChecks::IsNumericV<T>;
}