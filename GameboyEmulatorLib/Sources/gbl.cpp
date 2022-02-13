#include "pch.h"
#include "framework.h"

#include "gbl/allocator.h"
#include "gbl/core/mmu.h"
#include "gbl/core/cpu.h"
#include "gbl/gbl.h"
#include <fstream>
#include <cstdint>
#include <memory>

namespace
{

class DefaultAllocator : public gbl::IGBLAllocator
{
public:
    void* Allocate(const size_t size) override
    {
        return malloc(size);
    }

    void Free(void* ptr) override
    {
        free(ptr);
    }
};

static DefaultAllocator s_DefaultAllocator;

}

namespace gbl
{

GameBoyEmulator::GameBoyEmulator(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_MMU(nullptr)
    , m_CPU(nullptr)
{

}

GameBoyEmulator::GameBoyEmulator()
    : GameBoyEmulator(&s_DefaultAllocator)
{

}

GameBoyEmulator::~GameBoyEmulator()
{

}

bool GameBoyEmulator::Boot(const char* romFilePath)
{
    std::ifstream ifs;
    std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
    ifs.exceptions(exceptionMask);

    try
    {
        ifs.open(romFilePath, std::ios::in | std::ios::binary);
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

    const bool result = BootFromMemory(data, romSize);

    ifs.close();

    return result;
}

bool GameBoyEmulator::BootFromMemory(void* rom, const size_t romSize)
{
    gbl::Delete<MMU>(m_MMU, m_Allocator);
    gbl::Delete<CPU>(m_CPU, m_Allocator);

    m_MMU = gbl::New<MMU>(m_Allocator);
    m_CPU = gbl::New<CPU>(m_Allocator);

    return true;
}

void GameBoyEmulator::Shutdown()
{
    gbl::Delete<MMU>(m_MMU, m_Allocator);
    gbl::Delete<CPU>(m_CPU, m_Allocator);
}

} // namespace gbl
