#include "pch.h"
#include "framework.h"

#include "gbl/allocator.h"
#include "gbl/emulate/cartridge.h"
#include <fstream>
#include <cstdint>
#include <memory>

namespace gbl
{

Cartridge::Cartridge(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_Title(nullptr)
    , m_Data(nullptr)
    , m_Size(0)
{

}

Cartridge::~Cartridge()
{
    if (m_Data)
    {
        m_Allocator->Free(m_Data);
        m_Data = nullptr;
    }
    m_Size = 0;
}

bool Cartridge::Load(const char* filePath)
{
    std::ifstream ifs;
    std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
    ifs.exceptions(exceptionMask);

    try
    {
        ifs.open(filePath, std::ios::in | std::ios::binary);
    }
    catch (std::ios_base::failure&)
    {
        return false;
    }

    ifs.seekg(0, std::ios::end);
    const size_t romSize = ifs.tellg();
    ifs.seekg(0);

    char* data = static_cast<char*>(m_Allocator->Allocate(romSize));
    ifs.read(data, romSize);

    if (m_Data)
    {
        m_Allocator->Free(m_Data);
        m_Data = nullptr;
    }
    m_Size = 0;

    m_Data = static_cast<uint8_t*>(m_Allocator->Allocate(romSize));
    m_Size = static_cast<uint32_t>(romSize);
    memcpy(m_Data, data, m_Size);

    m_Allocator->Free(data);
    ifs.close();

    return true;
}

const uint8_t* Cartridge::GetData() const
{
    return m_Data;
}

uint32_t Cartridge::GetDataSize() const
{
    return m_Size;
}

} // namespace gbl
