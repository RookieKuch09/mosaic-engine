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
    struct IsNumeric<Types::F32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::F64>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::I8>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::I16>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UI32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::I64>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UI8>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UI16>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::I32>
    {
        static constexpr bool Result = true;
    };

    template <>
    struct IsNumeric<Types::UI64>
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