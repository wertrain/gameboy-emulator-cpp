#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }

namespace gbl
{

/// <summary>
/// LCD (Liquid Crystal Display) Interface
/// </summary>
class ILCD
{
public:
    virtual ~ILCD() {}

    virtual void DrawBegin() {};
    virtual void DrawPixel(int32_t x, int32_t y, uint32_t color) = 0;
    virtual void DrawEnd() {};
};

} // namespace gbl
