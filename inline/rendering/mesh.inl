#pragma once

#include "application/console.hpp"

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
    using ValueType = VectorTypeT<T>;
    constexpr uint32 ComponentCount = VectorElementCountV<T>;

    if constexpr (ComponentCount > 1)
    {
        Console::LogNotice("Detected a vector: Vector{}<{}>", ComponentCount, typeid(ValueType).name());
    }
    else
    {
        Console::LogNotice("Detected a scalar: {}", typeid(ValueType).name());
    }
}

template <typename... Args>
void Mosaic::Mesh::SetLayout(const MeshAttribute<Args>&... formats)
{
}

template <typename... Args>
void Mosaic::Mesh::SetVertexData(const std::vector<Args>&... data)
{
}