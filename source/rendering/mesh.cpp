#include "rendering/mesh.hpp"

#include "application/console.hpp"

#include <cstring>
#include <sstream>

namespace Mosaic::Internal::Rendering
{
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

    void Mesh::Submit()
    {
        Console::LogNotice("Mesh::Submit called");
        Console::LogNotice("Number of attributes: {}", static_cast<int>(mAttributes.size()));
        Console::LogNotice("Raw vertex data size: {} bytes", static_cast<int>(mRawData.size()));
        Console::LogNotice("Vertex length (stride): {} bytes", static_cast<int>(mVertexLengthBytes));

        if (mVertexLengthBytes == 0 || mRawData.empty())
        {
            Console::LogNotice("No vertex data to display.");
            return;
        }

        const Types::UI32 vertexCount = mRawData.size() / mVertexLengthBytes;

        for (Types::UI32 i = 0; i < vertexCount; ++i)
        {
            Console::LogNotice("Vertex {}:", i);

            const std::byte* vertexPtr = mRawData.data() + i * mVertexLengthBytes;

            for (const auto& attr : mAttributes)
            {
                std::ostringstream ss;
                const std::byte* attrPtr = vertexPtr + attr->OffsetBytes;

                Types::UI32 count = attr->LengthBytes / attr->TypeSize; // assume tightly packed
                ss << "[";
                for (Types::UI32 c = 0; c < count; ++c)
                {
                    if (c > 0)
                        ss << ", ";

                    switch (attr->EnumType)
                    {
                        case AttributeType::F32:
                        {
                            float value;
                            std::memcpy(&value, attrPtr + c * sizeof(float), sizeof(float));
                            ss << value;
                            break;
                        }
                        case AttributeType::UI32:
                        {
                            Types::UI32 value;
                            std::memcpy(&value, attrPtr + c * sizeof(Types::UI32), sizeof(Types::UI32));
                            ss << value;
                            break;
                        }
                        // Add other cases here if needed
                        default:
                            ss << "?";
                            break;
                    }
                }
                ss << "]";

                Console::LogNotice("  [Attr {} @ offset {}]: {}", attr->Index, attr->OffsetBytes, ss.str());
            }
        }
    }

    void Mesh::Unsubmit()
    {
    }
}