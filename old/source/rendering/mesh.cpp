#include "rendering/mesh.hpp"

#include "application/console.hpp"

namespace Mosaic::Internal::Rendering
{
    Mesh::Mesh()
        : mVertexLengthBytes(0), mFormat(nullptr), mSubmitted(false)
    {
    }

    void Mesh::SetVertexFormat(VertexFormat& format)
    {
        if (mFormat)
        {
            Console::LogWarning("Cannot rebind mesh to a new vertex format");
        }
        else if (format.mAttributes.empty())
        {
            Console::LogWarning("Cannot bind mesh to an empty vertex format");
        }
        else
        {

            mFormat = &format;

            for (Types::UI32 i = 0; i < mFormat->mAttributes.size(); i++)
            {
                auto& attribute = mFormat->mAttributes[i];

                attribute.Index = i;
                attribute.OffsetBytes = mVertexLengthBytes;

                mVertexLengthBytes += attribute.LengthBytes;
            }
        }
    }

    bool Mesh::CanSetVertexData() const
    {
        if (not mRawData.empty())
        {
            Console::LogWarning("Mesh vertex data cannot be redefined");
            return false;
        }

        if (not mFormat)
        {
            Console::LogWarning("Mesh must be bound to a vertex format before setting data");

            return false;
        }

        if (mFormat->mAttributes.empty())
        {
            Console::LogWarning("Vertex attributes must be defined prior to providing vertex data");

            return false;
        }

        return true;
    }

    void Mesh::Submit()
    {
        if (mSubmitted)
        {
            Console::LogWarning("Mesh already submitted, cannot resubmit");

            return;
        }

        if (mRawData.empty())
        {
            Console::LogWarning("Mesh must have valid data before being submitted");

            return;
        }

        if (not mFormat)
        {
            Console::LogWarning("Mesh must be bound to a vertex format before submitting");

            return;
        }

        if (mFormat->mAttributes.empty())
        {
            Console::LogError("Vertex attributes must be defined before submitting");

            return;
        }
    }

    void Mesh::Unsubmit()
    {
        if (not mSubmitted)
        {
            Console::LogWarning("Mesh already desubmitted");

            return;
        }
    }
}