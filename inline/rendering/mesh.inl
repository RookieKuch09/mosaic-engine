#pragma once

#include "rendering/mesh.hpp"

#include "application/console.hpp"

namespace Mosaic::Internal::Rendering
{
    template <typename T>
    MeshAttribute<T>::MeshAttribute(Types::UI32 count)
    {
        GetTypeData(count);
    }

    template <typename T>
    void MeshAttribute<T>::GetTypeData(Types::UI32 count)
    {
        constexpr Types::UI32 typeCount = AttributeInfo<T>::Count;
        constexpr Types::UI32 typeSize = AttributeInfo<T>::TypeSize;

        EnumType = AttributeInfo<T>::EnumType;
        LengthBytes = typeCount * typeSize * count;
        TypeSize = typeSize;
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

            Types::UI32 index = 0;

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

        constexpr Types::UI64 NumInputs = sizeof...(Args);

        if (not ValidateAttributeCount<NumInputs>())
        {
            return;
        }

        std::array<Types::UI32, NumInputs> attributeCounts = {};

        if (not ValidateAttributeData<Args...>(data..., attributeCounts))
        {
            return;
        }

        if (not ValidateVertexCounts(attributeCounts))
        {
            return;
        }

        const Types::UI32 vertexCount = attributeCounts[0];

        mRawData.reserve(vertexCount * mVertexLengthBytes);

        const auto dataTuple = std::tie(data...);
        for (Types::UI32 vertex = 0; vertex < vertexCount; vertex++)
        {
            InterleaveVertexData(vertex, std::make_index_sequence<NumInputs>{}, dataTuple);
        }
    }

    template <Types::UI64 NumInputs>
    bool Mesh::ValidateAttributeCount() const
    {
        if (NumInputs != mAttributes.size())
        {
            Console::LogWarning("Attribute count and vertex data input count do not match");
            return false;
        }

        return true;
    }

    template <typename... Args>
    bool Mesh::ValidateAttributeData(const std::vector<Args>&... data, std::array<Types::UI32, sizeof...(Args)>& outCounts)
    {
        bool mismatch = false;
        Types::UI32 index = 0;

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

            Types::UI32 vectorSizeBytes = vec.size() * sizeof(ArrayType);
            Types::UI32 attributeLengthBytes = mAttributes[index]->LengthBytes;

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

    template <Types::UI64 NumInputs>
    bool Mesh::ValidateVertexCounts(const std::array<Types::UI32, NumInputs>& counts)
    {
        const Types::UI32 vertexCount = counts[0];
        for (Types::UI32 i = 1; i < counts.size(); ++i)
        {
            if (vertexCount != counts[i])
            {
                Console::LogWarning("Mismatch in attribute counts for the given data");
                return false;
            }
        }
        return true;
    }

    template <Types::UI64... NumInputs, typename... Args>
    void Mesh::InterleaveVertexData(Types::UI32 vertex, std::index_sequence<NumInputs...>, const std::tuple<const std::vector<Args>&...>& dataTuple)
    {
        (void)std::initializer_list<int>{
            ([&]
             {
            const auto& vec = std::get<NumInputs>(dataTuple);
            const auto& attribute = mAttributes[NumInputs];

            using VecT = std::decay_t<decltype(vec)>;
            using ElemT = typename VecT::value_type;
            constexpr bool isRaw = std::is_arithmetic_v<ElemT>;

            Types::UI32 elementCount = attribute->LengthBytes / sizeof(typename AttributeInfo<ElemT>::Type);

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