#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <memory>
#include <vector>

namespace Mosaic::Internal::Rendering
{
    enum class AttributeType
    {
        F32,
        F64,
        I8,
        I16,
        I32,
        I64,
        UI8,
        UI16,
        UI32,
        UI64,
    };

    template <TypeConcepts::Numeric T>
    struct AttributeTypeMapping
    {
    };

    template <>
    struct AttributeTypeMapping<Types::F32>
    {
        static constexpr AttributeType Value = AttributeType::F32;
    };
    template <>
    struct AttributeTypeMapping<Types::F64>
    {
        static constexpr AttributeType Value = AttributeType::F64;
    };

    template <>
    struct AttributeTypeMapping<Types::I8>
    {
        static constexpr AttributeType Value = AttributeType::I8;
    };
    template <>
    struct AttributeTypeMapping<Types::I16>
    {
        static constexpr AttributeType Value = AttributeType::I16;
    };
    template <>
    struct AttributeTypeMapping<Types::UI32>
    {
        static constexpr AttributeType Value = AttributeType::UI32;
    };
    template <>
    struct AttributeTypeMapping<Types::I64>
    {
        static constexpr AttributeType Value = AttributeType::I64;
    };

    template <>
    struct AttributeTypeMapping<Types::UI8>
    {
        static constexpr AttributeType Value = AttributeType::UI8;
    };
    template <>
    struct AttributeTypeMapping<Types::UI16>
    {
        static constexpr AttributeType Value = AttributeType::UI16;
    };
    template <>
    struct AttributeTypeMapping<Types::I32>
    {
        static constexpr AttributeType Value = AttributeType::UI32;
    };
    template <>
    struct AttributeTypeMapping<Types::UI64>
    {
        static constexpr AttributeType Value = AttributeType::UI64;
    };

    template <typename T>
    struct AttributeInfo
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = 1;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 2>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = 2;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 3>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = 3;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T>
    struct AttributeInfo<Types::Vector<T, 4>>
    {
        static constexpr AttributeType EnumType = AttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = 4;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    class MeshAttributeBase
    {
    protected:
        AttributeType EnumType;
        Types::UI32 LengthBytes;
        Types::UI32 OffsetBytes;
        Types::UI32 Index;
        Types::UI32 TypeSize;

        friend class Mesh;
    };

    template <typename T>
    class MeshAttribute : public MeshAttributeBase
    {
    public:
        MeshAttribute() = delete;
        MeshAttribute(Types::UI32 count);

    private:
        void GetTypeData(Types::UI32 count);
    };

    class Mesh
    {
    public:
        template <typename... Args>
        inline void SetLayout(const MeshAttribute<Args>&... formats);

        template <typename... Args>
        inline void SetVertexData(const std::vector<Args>&... data);

        void Submit();
        void Unsubmit();

    private:
        bool CanSetVertexData() const;

        template <Types::UI64 NumInputs>
        bool ValidateAttributeCount() const;

        template <typename... Args>
        bool ValidateAttributeData(const std::vector<Args>&... data, std::array<Types::UI32, sizeof...(Args)>& outCounts);

        template <Types::UI64 NumInputs>
        bool ValidateVertexCounts(const std::array<Types::UI32, NumInputs>& counts);

        template <Types::UI64... NumInputs, typename... Args>
        void InterleaveVertexData(Types::UI32 vertex, std::index_sequence<NumInputs...>, const std::tuple<const std::vector<Args>&...>& dataTuple);

        std::vector<std::unique_ptr<MeshAttributeBase>> mAttributes;
        std::vector<std::byte> mRawData;

        Types::UI32 mVertexLengthBytes;
    };
}

#include "rendering/mesh.inl"
