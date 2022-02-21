#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }

namespace gbl
{

class Cartridge
{
public:
    Cartridge(IGBLAllocator* allocator);
    ~Cartridge();

    bool Load(const char* filePath);
    const uint8_t* GetData() const;
    uint32_t GetDataSize() const;

private:
    IGBLAllocator* m_Allocator;
    uint8_t* m_Title;
    uint8_t* m_Data;
    uint32_t m_Size;
};

} // namespace gbl
