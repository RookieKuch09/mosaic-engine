#pragma once

#include "rendering/mesh.hpp"

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

    AttributeLength = ComponentCount * TypeSize;
    Type = RawTypeMap<InputType>::value;
}

template <typename... Args>
void Mosaic::Mesh::SetLayout(const MeshAttribute<Args>&... formats)
{
}

template <typename... Args>
void Mosaic::Mesh::SetVertexData(const std::vector<Args>&... data)
{
}