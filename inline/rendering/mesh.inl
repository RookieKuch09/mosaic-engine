#pragma once

#include "rendering/mesh.hpp"

#include "application/console.hpp"

#include "utilities/vector.hpp"

template <typename T>
Mosaic::MeshAttribute<T>::MeshAttribute()
{
    GetTypeData();
}

template <typename T>
void Mosaic::MeshAttribute<T>::GetTypeData()
{
    using InputType = VectorTypeT<T>;

    static_assert(std::is_arithmetic_v<InputType>, "MeshAttribute only supports arithmetic or Vector of arithmetic types");

    constexpr uint32 ComponentCount = VectorElementCountV<T>;
    constexpr uint32 TypeSize = sizeof(InputType);

    AttributeLengthBytes = ComponentCount * TypeSize;
    Type = RawTypeMap<InputType>::value;
}

template <typename... Args>
void Mosaic::Mesh::SetLayout(const MeshAttribute<Args>&... formats)
{
    if (not mAttributes.empty())
    {
        Console::LogWarning("Mesh attributes cannot be redefined");
    }
    else
    {
        (..., mAttributes.push_back(static_cast<const MeshAttributeBase&>(formats)));

        uint32 index = 0;
        mVertexLengthBytes = 0;

        for (auto& attribute : mAttributes)
        {
            attribute.AttributeOffsetBytes = mVertexLengthBytes;
            attribute.AttributeIndex = index;

            mVertexLengthBytes += attribute.AttributeLengthBytes;

            index++;
        }
    }
}

template <typename... Args>
void Mosaic::Mesh::SetVertexData(const std::vector<Args>&... data)
{
    if (not mRawData.empty())
    {
        Console::LogWarning("Mesh vertex data cannot be redefined");
    }
    else if (mAttributes.empty())
    {
        Console::LogWarning("Mesh attributes must be defined prior to providing vertex data");
    }
    else
    {
        constexpr uint32 NumInputs = sizeof...(Args);

        if (NumInputs != mAttributes.size())
        {
            Console::LogWarning("Attribute count and vertex data input count do not match");
            return;
        }

        auto groupable = [&](auto& vec, unsigned int i)
        {
            uint32& attributeLength = mAttributes[i].AttributeLengthBytes;
            uint32 vectorSizebytes = 0;

            using VectorType = std::decay_t<decltype(vec[0])>;
        };

        uint32 vertexCount = 0;

        bool first = true;
        bool sizeMismatch = false;

        ((first ? (vertexCount = data.size(), first = false) : (sizeMismatch |= (data.size() != vertexCount))), ...);

        if (sizeMismatch)
        {
            // try formatting the mismatched data as groups, as it may be raw
        }

        mRawData.reserve(vertexCount * mVertexLengthBytes);

        for (uint32 i = 0; i < vertexCount; i++)
        {
            ([&]
             {
            const auto& vec = data;
            const auto& value = vec[i];

            using T = std::decay_t<decltype(value)>;
            using ElemT = VectorTypeT<T>;
            constexpr size_t Count = VectorElementCountV<T>;

            const std::byte* bytes = reinterpret_cast<const std::byte*>(
                Count == 1 ? static_cast<const void*>(&value)
                           : static_cast<const void*>(&value[0])
            );

            mRawData.insert(mRawData.end(), bytes, bytes + sizeof(ElemT) * Count); }(), ...);
        }
    }
}

template <typename T>
void Mosaic::Mesh::FlattenToRawBytes(const std::vector<T>& input)
{
    using ElementType = VectorTypeT<T>;
    constexpr size_t ComponentCount = VectorElementCountV<T>;
    constexpr size_t TotalBytes = sizeof(ElementType) * ComponentCount;

    for (const auto& value : input)
    {
        if constexpr (ComponentCount == 1)
        {
            const std::byte* bytes = reinterpret_cast<const std::byte*>(&value);
            mRawData.insert(mRawData.end(), bytes, bytes + sizeof(ElementType));
        }
        else
        {
            const ElementType* components = reinterpret_cast<const ElementType*>(&value);
            const std::byte* bytes = reinterpret_cast<const std::byte*>(components);
            mRawData.insert(mRawData.end(), bytes, bytes + TotalBytes);
        }
    }
}