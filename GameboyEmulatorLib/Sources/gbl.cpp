#include "pch.h"
#include "framework.h"

#include "gbl/allocator.h"
#include "gbl/core/mmu.h"
#include "gbl/core/ppu.h"
#include "gbl/core/cpu.h"
#include "gbl/cartridge.h"
#include "gbl/interface.h"
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
    , m_PPU(nullptr)
    , m_CPU(nullptr)
    , m_LCD(nullptr)
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
    Cartridge cartridge(m_Allocator);

    if (!cartridge.Load(romFilePath))
    {
        return false;
    }

    return BootFromCartridge(&cartridge);
}

bool GameBoyEmulator::BootFromCartridge(const Cartridge* cartridge)
{
    gbl::Delete<MMU>(m_MMU, m_Allocator);
    gbl::Delete<PPU>(m_PPU, m_Allocator);
    gbl::Delete<CPU>(m_CPU, m_Allocator);

    m_MMU = gbl::New<MMU>(m_Allocator);
    m_PPU = gbl::New<PPU>(m_Allocator);
    m_CPU = gbl::New<CPU>(m_Allocator);

    m_PPU->Create(m_MMU);

    m_MMU->LoadCartridge(cartridge);
    m_MMU->LoadBIOS();
    m_CPU->RunCartridge(m_MMU, m_PPU);

    Run();

    m_PPU->Destroy();

    return true;
}

void GameBoyEmulator::Run()
{
    while (1)
    {
        m_CPU->Tick(m_MMU);
        m_PPU->Tick(m_CPU, m_MMU);

        UpdateLCD();
    }
}

void GameBoyEmulator::UpdateLCD()
{
    if (m_LCD)
    {
        if (m_PPU->CanRender())
        {
            m_LCD->DrawBegin();
            for (int y = 0; y < PPU::BUFFER_HEIGHT; y++)
            {
                for (int x = 0; x < PPU::BUFFER_WIDTH; x++)
                {
                    const auto color = m_PPU->GetFrameBuffer(x, y);
                    m_LCD->DrawPixel(x, y, color);
                }
            }
            m_LCD->DrawEnd();

            m_PPU->ResetCanRender();
            m_PPU->ResetFrameBuffer();
        }
    }
}

void GameBoyEmulator::Shutdown()
{
    gbl::Delete<MMU>(m_MMU, m_Allocator);
    gbl::Delete<PPU>(m_PPU, m_Allocator);
    gbl::Delete<CPU>(m_CPU, m_Allocator);
}

void GameBoyEmulator::SetLCD(ILCD* lcd)
{
    m_LCD = lcd;
}

} // namespace gbl
