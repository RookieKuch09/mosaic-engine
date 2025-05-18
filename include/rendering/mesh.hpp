#pragma once

#include "utilities/arithmetic.hpp"

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

        RawType Type;
        uint32 AttributeLength;
        uint32 VertexLength;
        uint32 AttributeOffset;
        uint32 AttributeIndex;

        friend class Mesh;
    };

    template <typename T>
    class MeshAttribute : public MeshAttributeBase
    {
    public:
        MeshAttribute();

        using Type = T;

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
        std::vector<MeshAttributeBase> mAttributes;
    };
}

#include "rendering/mesh.inl"
