#pragma once

#include "rendering/mesh.hpp"

#include "application/console.hpp"

#include <cstring>

namespace Mosaic::Internal::Rendering
{
    template <typename T>
    MeshAttribute<T>::MeshAttribute(Types::UInt32 count)
    {
        GetTypeData(count);
    }

    template <typename T>
    void MeshAttribute<T>::GetTypeData(Types::UInt32 count)
    {
        constexpr Types::UInt32 TypeCount = AttributeInfo<T>::Count;
        constexpr Types::UInt32 TypeSize = AttributeInfo<T>::TypeSize;

        EnumType = AttributeInfo<T>::EnumType;
        LengthBytes = TypeCount * TypeSize * count;
    }

    template <typename... Args>
    void Mesh::SetLayout(const MeshAttribute<Args>&... formats)
    {
        if (not mAttributes.empty())
        {
            Console::LogWarning("Mesh attributes cannot be redefined");
        }
        else
        {
            (..., mAttributes.push_back(std::make_unique<MeshAttribute<Args>>(formats)));

            Types::UInt32 index = 0;

            mVertexLengthBytes = 0;

            for (auto& attribute : mAttributes)
            {
                attribute->OffsetBytes = mVertexLengthBytes;
                attribute->Index = index;

                mVertexLengthBytes += attribute->LengthBytes;

                index++;
            }
        }
    }

    template <typename... Args>
    void Mesh::SetVertexData(const std::vector<Args>&... data)
    {
        if (not CanSetVertexData())
        {
            return;
        }

        constexpr Types::UInt32 NumInputs = sizeof...(Args);

        if (not ValidateAttributeCount<NumInputs>())
        {
            return;
        }

        std::array<Types::UInt32, NumInputs> attributeCounts = {};

        if (not ValidateAttributeData(data..., attributeCounts))
        {
            return;
        }

        if (not ValidateVertexCounts(attributeCounts))
        {
            return;
        }

        const Types::UInt32 vertexCount = attributeCounts[0];

        mRawData.reserve(vertexCount * mVertexLengthBytes);

        const auto dataTuple = std::tie(data...);
        for (Types::UInt32 vertex = 0; vertex < vertexCount; vertex++)
        {
            InterleaveVertexData(vertex, std::make_index_sequence<NumInputs>{}, dataTuple);
        }
    }

    bool Mesh::CanSetVertexData() const
    {
        if (not mRawData.empty())
        {
            Console::LogWarning("Mesh vertex data cannot be redefined");
            return false;
        }

        if (mAttributes.empty())
        {
            Console::LogWarning("Mesh attributes must be defined prior to providing vertex data");
            return false;
        }

        return true;
    }

    template <Types::UInt32 NumInputs>
    bool Mesh::ValidateAttributeCount() const
    {
        if (NumInputs != mAttributes.size())
        {
            Console::LogWarning("Attribute count and vertex data input count do not match");
            return false;
        }

        return true;
    }

    template <typename... Args, Types::UInt32 NumInputs>
    bool Mesh::ValidateAttributeData(const std::vector<Args>&... data, std::array<Types::UInt32, NumInputs>& outCounts)
    {
        bool mismatch = false;
        Types::UInt32 index = 0;

        auto check = [&](const auto& vec)
        {
            if (vec.empty())
            {
                Console::LogWarning("Cannot provide empty data for mesh attribute");
                mismatch = true;
                return;
            }

            if (index >= mAttributes.size())
            {
                Console::LogWarning("Too much data provided for mesh");
                mismatch = true;
                return;
            }

            using ArrayType = std::decay_t<decltype(vec[0])>;
            using BaseType = AttributeInfo<ArrayType>::Type;

            if (AttributeInfo<BaseType>::EnumType != mAttributes[index]->EnumType)
            {
                Console::LogWarning("Type mismatch between attribute and provided data");
                mismatch = true;
                return;
            }

            Types::UInt32 vectorSizeBytes = vec.size() * sizeof(ArrayType);
            Types::UInt32 attributeLengthBytes = mAttributes[index]->LengthBytes;

            if (vectorSizeBytes % attributeLengthBytes != 0)
            {
                Console::LogWarning("Data for attribute index {} is misaligned", index);
                mismatch = true;
                return;
            }

            outCounts[index] = vectorSizeBytes / attributeLengthBytes;
            index++;
        };

        (check(data), ...);

        return not mismatch;
    }

    template <Types::UInt32 NumInputs>
    bool Mesh::ValidateVertexCounts(const std::array<Types::UInt32, NumInputs>& counts)
    {
        const Types::UInt32 vertexCount = counts[0];
        for (Types::UInt32 i = 1; i < counts.size(); ++i)
        {
            if (vertexCount != counts[i])
            {
                Console::LogWarning("Mismatch in attribute counts for the given data");
                return false;
            }
        }
        return true;
    }

    template <size_t... Is, typename... Args>
    void Mesh::InterleaveVertexData(Types::UInt32 vertex, std::index_sequence<Is...>, const std::tuple<const std::vector<Args>&...>& dataTuple)
    {
        (void)std::initializer_list<int>{
            ([&]
             {
            const auto& vec = std::get<Is>(dataTuple);
            const auto& attribute = mAttributes[Is];

            using VecT = std::decay_t<decltype(vec)>;
            using ElemT = typename VecT::value_type;
            constexpr bool isRaw = std::is_arithmetic_v<ElemT>;

            Types::UInt32 elementCount = attribute->LengthBytes / sizeof(AttributeInfo<ElemT>::Type);

            if constexpr (isRaw)
            {
                const auto* ptr = &vec[vertex * elementCount];
                const std::byte* bytes = reinterpret_cast<const std::byte*>(ptr);
                mRawData.insert(mRawData.end(), bytes, bytes + elementCount * sizeof(ElemT));
            }
            else
            {
                const auto& value = vec[vertex];
                const std::byte* bytes = reinterpret_cast<const std::byte*>(&value);
                mRawData.insert(mRawData.end(), bytes, bytes + attribute->LengthBytes);
            }

            return 0; }(), 0)...};
    }
}