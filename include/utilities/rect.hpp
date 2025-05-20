#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

namespace Mosaic::Internal::Types
{
    struct Rectangle
    {
        Vector2<UI32> Position;
        Vector2<UI32> Size;
    };
}