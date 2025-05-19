#pragma once

#include "utilities/arithmetic.hpp"

#include <variant>
#include <vector>

namespace Mosaic
{
    class MeshAttributeBase
    {
    protected:
        enum class RawType
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

        template <typename T>
        struct RawTypeMap;

        template <>
        struct RawTypeMap<float32>
        {
            static constexpr RawType value = RawType::Float32;
        };
        template <>
        struct RawTypeMap<float64>
        {
            static constexpr RawType value = RawType::Float64;
        };

        template <>
        struct RawTypeMap<int8>
        {
            static constexpr RawType value = RawType::Int8;
        };
        template <>
        struct RawTypeMap<int16>
        {
            static constexpr RawType value = RawType::Int16;
        };
        template <>
        struct RawTypeMap<int32>
        {
            static constexpr RawType value = RawType::Int32;
        };
        template <>
        struct RawTypeMap<int64>
        {
            static constexpr RawType value = RawType::Int64;
        };

        template <>
        struct RawTypeMap<uint8>
        {
            static constexpr RawType value = RawType::UInt8;
        };
        template <>
        struct RawTypeMap<uint16>
        {
            static constexpr RawType value = RawType::UInt16;
        };
        template <>
        struct RawTypeMap<uint32>
        {
            static constexpr RawType value = RawType::UInt32;
        };
        template <>
        struct RawTypeMap<uint64>
        {
            static constexpr RawType value = RawType::UInt64;
        };

        template <typename T>
        struct RawTypeMap;

        RawType Type;
        uint32 AttributeLengthBytes;
        uint32 AttributeOffsetBytes;
        uint32 AttributeIndex;

        friend class Mesh;
    };

    template <typename T>
    class MeshAttribute : public MeshAttributeBase
    {
    public:
        MeshAttribute();

        using ValueType = T;

    private:
        void GetTypeData();
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
        template <typename T>
        void FlattenToRawBytes(const std::vector<T>& input);

        std::vector<MeshAttributeBase> mAttributes;
        std::vector<std::byte> mRawData;

        uint32 mVertexLengthBytes;
    };
}

#include "rendering/mesh.inl"
