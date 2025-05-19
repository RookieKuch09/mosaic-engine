#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <memory>
#include <vector>

namespace Mosaic::Internal::Rendering
{
    enum class AttributeType
    {
        Float32,
        Float64,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt8,
        UInt16,
        UInt32,
        UInt64,
    };

    template <TypeConcepts::Numeric T>
    struct AttributeTypeMapping
    {
    };

    template <>
    struct AttributeTypeMapping<Types::Float32>
    {
        static constexpr AttributeType Value = AttributeType::Float32;
    };
    template <>
    struct AttributeTypeMapping<Types::Float64>
    {
        static constexpr AttributeType Value = AttributeType::Float64;
    };

    template <>
    struct AttributeTypeMapping<Types::Int8>
    {
        static constexpr AttributeType Value = AttributeType::Int8;
    };
    template <>
    struct AttributeTypeMapping<Types::Int16>
    {
        static constexpr AttributeType Value = AttributeType::Int16;
    };
    template <>
    struct AttributeTypeMapping<Types::Int32>
    {
        static constexpr AttributeType Value = AttributeType::Int32;
    };
    template <>
    struct AttributeTypeMapping<Types::Int64>
    {
        static constexpr AttributeType Value = AttributeType::Int64;
    };

    template <>
    struct AttributeTypeMapping<Types::UInt8>
    {
        static constexpr AttributeType Value = AttributeType::UInt8;
    };
    template <>
    struct AttributeTypeMapping<Types::UInt16>
    {
        static constexpr AttributeType Value = AttributeType::UInt16;
    };
    template <>
    struct AttributeTypeMapping<Types::UInt32>
    {
        static constexpr AttributeType Value = AttributeType::UInt32;
    };
    template <>
    struct AttributeTypeMapping<Types::UInt64>
    {
        static constexpr AttributeType Value = AttributeType::UInt64;
    };

    template <typename T>
    struct AttributeInfo
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UInt32 Count = 1;
        static constexpr Types::UInt32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 2>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UInt32 Count = 2;
        static constexpr Types::UInt32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 3>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UInt32 Count = 3;
        static constexpr Types::UInt32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 4>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UInt32 Count = 4;
        static constexpr Types::UInt32 TypeSize = sizeof(T);
        using Type = T;
    };

    class MeshAttributeBase
    {
    protected:
        AttributeType EnumType;
        Types::UInt32 LengthBytes;
        Types::UInt32 OffsetBytes;
        Types::UInt32 Index;

        friend class Mesh;
    };

    template <typename T>
    class MeshAttribute : public MeshAttributeBase
    {
    public:
        MeshAttribute() = delete;
        MeshAttribute(Types::UInt32 count);

    private:
        void GetTypeData(Types::UInt32 count);
    };

    class Mesh
    {
    public:
        template <typename... Args>
        void SetLayout(const MeshAttribute<Args>&... formats);

        template <typename... Args>
        void SetVertexData(const std::vector<Args>&... data);

        void Submit();
        void Unsubmit();

    private:
        bool CanSetVertexData() const;

        template <Types::UInt32 NumInputs>
        bool ValidateAttributeCount() const;

        template <typename... Args, Types::UInt32 NumInputs>
        bool ValidateAttributeData(const std::vector<Args>&... data, std::array<Types::UInt32, NumInputs>& outCounts);

        template <Types::UInt32 NumInputs>
        bool ValidateVertexCounts(const std::array<Types::UInt32, NumInputs>& counts);

        template <size_t... Is, typename... Args>
        void InterleaveVertexData(Types::UInt32 vertex, std::index_sequence<Is...>, const std::tuple<const std::vector<Args>&...>& dataTuple);

        std::vector<std::unique_ptr<MeshAttributeBase>> mAttributes;
        std::vector<std::byte> mRawData;

        Types::UInt32 mVertexLengthBytes;
    };
}

#include "rendering/mesh.inl"
