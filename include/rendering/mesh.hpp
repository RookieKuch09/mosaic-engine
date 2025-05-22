#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <vector>

namespace Mosaic::Internal::Rendering
{
    enum class VertexAttributeType
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
    struct VertexAttributeTypeMapping
    {
    };

    template <>
    struct VertexAttributeTypeMapping<Types::F32>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::F32;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::F64>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::F64;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::I8>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::I8;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::I16>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::I16;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::UI32>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::UI32;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::I64>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::I64;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::UI8>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::UI8;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::UI16>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::UI16;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::I32>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::UI32;
    };

    template <>
    struct VertexAttributeTypeMapping<Types::UI64>
    {
        static constexpr VertexAttributeType Value = VertexAttributeType::UI64;
    };

    template <typename T>
    struct AttributeInfo
    {
        static constexpr VertexAttributeType EnumType = VertexAttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = 1;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    template <TypeConcepts::Numeric T, Types::UI32 N>
    struct AttributeInfo<Types::Vec<T, N>>
    {
        static constexpr VertexAttributeType EnumType = VertexAttributeTypeMapping<T>::Value;
        static constexpr Types::UI32 Count = N;
        static constexpr Types::UI32 TypeSize = sizeof(T);
        using Type = T;
    };

    class VertexAttributeBase
    {
    protected:
        VertexAttributeType EnumType;
        Types::UI32 LengthBytes;
        Types::UI32 OffsetBytes;
        Types::UI32 Index;
        Types::UI32 TypeSize;
        Types::UI32 Count;

        friend class Mesh;
    };

    template <typename T>
    class VertexAttribute : public VertexAttributeBase
    {
    public:
        VertexAttribute() = delete;
        VertexAttribute(Types::UI32 count);

    private:
        void GetTypeData(Types::UI32 count);
    };

    class VertexFormat
    {
    public:
        template <typename T>
        VertexFormat& AddAttribute(VertexAttribute<T>&& attribute);

        template <typename T>
        void AddAttribute(const VertexAttribute<T>& attribute) = delete;

    private:
        std::vector<VertexAttributeBase> mAttributes;

        friend class Mesh;
    };

    class Buffer
    {
    };

    class Mesh
    {
    public:
        Mesh();

        void SetVertexFormat(VertexFormat& format);

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

        VertexFormat* mFormat;

        std::vector<std::byte> mRawData;

        Types::UI32 mVertexLengthBytes;

        bool mSubmitted;
    };
}

#include "rendering/mesh.inl"
