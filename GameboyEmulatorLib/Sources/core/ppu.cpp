#include "pch.h"
#include "framework.h"

#include <memory>
#include "gbl/allocator.h"
#include "gbl/core/mmu.h"
#include "gbl/core/cpu.h"
#include "gbl/core/ppu.h"

namespace
{

constexpr uint16_t PPU_TIMES[4] = { 80, 172, 204, 456 };
constexpr float PPU_SCALE = 1.0f;

}

namespace gbl
{

PPU::PPU(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_PPU(nullptr)
{
    m_PPU = static_cast<PPU_t*>(m_Allocator->Allocate(sizeof(PPU_t)));
    memset(m_PPU, 0, sizeof(PPU_t));
}

PPU::~PPU()
{
    if (m_PPU)
    {
        free(m_PPU);
        m_PPU = nullptr;
    }
}

bool PPU::Create(MMU* mmu)
{
    if (mmu == nullptr)
    {
        return false;
    }

    auto mmuv = mmu->GetMMU();
    m_PPU->vram = mmuv->vram;
    m_PPU->oam = mmuv->oam;

    m_PPU->control = reinterpret_cast<PPU_t::ControlType*>(mmuv->ppu);

    m_PPU->mode = PPU_MODE_OAM;
    m_PPU->clockCount = 0;

    m_PPU->scrollY = reinterpret_cast<Register_t*>(mmuv->ppu + 0x02);
    m_PPU->scrollX = reinterpret_cast<Register_t*>(mmuv->ppu + 0x03);
    m_PPU->line = reinterpret_cast<Register_t*>(mmuv->ppu + 0x04);
    m_PPU->windowY = reinterpret_cast<Register_t*>(mmuv->ppu + 0x0a);
    m_PPU->windowX = reinterpret_cast<Register_t*>(mmuv->ppu + 0x0b);

    m_PPU->scrollX->value = m_PPU->scrollY->value = m_PPU->line->value = 0;

    m_PPU->colors[0] = 0xFFFFFFFF;
    m_PPU->colors[1] = 0xFFC0C0C0;
    m_PPU->colors[2] = 0xFF606060;
    m_PPU->colors[3] = 0xFF000000;

    // フレームバッファの初期化
    m_PPU->canRender = true;
    ResetFrameBuffer();

    memset(m_PPU->tileset, 0, sizeof(m_PPU->tileset));
    return true;
}

void PPU::Destroy()
{

}

void PPU::EnableLCD()
{
    m_PPU->control->lcdDisplay = true;
}

bool PPU::CanRender() const
{
    return m_PPU->canRender;
}

uint32_t PPU::GetFrameBuffer(const int32_t x, const int32_t y) const
{
    return m_PPU->framebuffer[y][x];
}

void PPU::ResetCanRender()
{
    m_PPU->canRender = false;
}

void PPU::ResetFrameBuffer()
{
    memset(m_PPU->framebuffer, 0, BUFFER_HEIGHT * BUFFER_WIDTH * sizeof(uint32_t));
}

void PPU::Tick(CPU* cpu, MMU* mmu)
{
    UpdateMemory(mmu);

    // if (!m_PPU->control->lcdDisplay)
    // {
    //     return;
    // }

    m_PPU->clockCount += cpu->GetCurrentTick();

    switch (m_PPU->mode)
    {
    case PPU_MODE_OAM:
        if (m_PPU->clockCount >= (uint16_t)(PPU_TIMES[0] * PPU_SCALE))
        {
            m_PPU->clockCount %= (uint16_t)(PPU_TIMES[0] * PPU_SCALE);
            m_PPU->mode = PPU_MODE_VRAM;
        }
        break;
    case PPU_MODE_VRAM:
        if (m_PPU->clockCount >= (uint16_t)(PPU_TIMES[1] * PPU_SCALE))
        {
            m_PPU->clockCount %= (uint16_t)(PPU_TIMES[1] * PPU_SCALE);
            m_PPU->mode = PPU_MODE_HBLANK;

            RenderLine(mmu);
        }
        break;
    case PPU_MODE_HBLANK:
        if (m_PPU->clockCount >= (uint16_t)(PPU_TIMES[2] * PPU_SCALE))
        {
            m_PPU->clockCount %= (uint16_t)(PPU_TIMES[2] * PPU_SCALE);
            m_PPU->line->value++;

            if (m_PPU->line->value == PPU_HLINES)
            {
                m_PPU->mode = PPU_MODE_VBLANK;
                m_PPU->canRender = true;
                (*mmu->GetMMU()->intFlags) |= CPU::CPU_INT_VBLANK;
            }
            else
            {
                m_PPU->mode = PPU_MODE_OAM;
            }
        }
        break;
    case PPU_MODE_VBLANK:
        if (m_PPU->clockCount >= (uint16_t)(PPU_TIMES[3] * PPU_SCALE))
        {
            m_PPU->clockCount %= (uint16_t)(PPU_TIMES[3] * PPU_SCALE);
            m_PPU->line->value++;

            if (m_PPU->line->value >= PPU_VLINES)
            {
                // TODO:
                //log_message("[PPU] Leaving VBLANK");
                m_PPU->mode = PPU_MODE_OAM;
                m_PPU->line->value = 0;
            }
        }
        break;
    }
}

void PPU::UpdateMemory(MMU* mmu)
{
    if (mmu->GetMMU()->lastWritten.empty) return;

    if (mmu->GetMMU()->lastWritten.address == 0xFF47)
    {
        // Update palette
        m_PPU->palette[3] = (mmu->GetMMU()->lastWritten.d8 & 0xc0) >> 6;
        m_PPU->palette[2] = (mmu->GetMMU()->lastWritten.d8 & 0x30) >> 4;
        m_PPU->palette[1] = (mmu->GetMMU()->lastWritten.d8 & 0x0c) >> 2;
        m_PPU->palette[0] = (mmu->GetMMU()->lastWritten.d8 & 0x03);
        mmu->GetMMU()->lastWritten.empty = true;
    }
    else if (mmu->GetMMU()->lastWritten.address >= 0x8000 && mmu->GetMMU()->lastWritten.address < 0xA000)
    {
        // Update tileset
        UpdateTile(mmu->GetMMU()->lastWritten.address, mmu->GetMMU()->lastWritten.d8);
        mmu->GetMMU()->lastWritten.empty = true;
    }
}

void PPU::UpdateTile(uint16_t addr, uint8_t data)
{
    const uint16_t vaddr = addr & 0x1FFE;
    const uint16_t index = (vaddr >> 4);
    const uint8_t y = (vaddr >> 1) & 7;

    const uint8_t v = data;
    uint8_t v1 = m_PPU->vram[vaddr + 1];

    for (uint8_t x = 0; x < 8; x++)
    {
        uint8_t sx = 1 << (7 - x);
        m_PPU->tileset[index][y][x] = (v & sx ? 1 : 0) + (v1 & sx ? 2 : 0);
    }
}

void PPU::RenderLine(MMU* mmu)
{
    uint16_t mapoffset = m_PPU->control->bgWinTiledataSelect ? 0x1C00 : 0x1800;
    mapoffset += (((m_PPU->line->value + m_PPU->scrollY->value) & 255) >> 3) << 5;

    const uint8_t lineoffset = (m_PPU->scrollX->value >> 3);
    const uint8_t y = (m_PPU->line->value + m_PPU->scrollY->value) & 7;

    for (uint8_t x = 0; x < BUFFER_WIDTH; x++)
    {
        int8_t tile = static_cast<int8_t>(m_PPU->vram[mapoffset + lineoffset + (x / 8)]);

        if (m_PPU->control->bgTilemapSelect && tile < 128)
        {
            // TODO: 
            tile += 0x100;
        }

        // TODO: 
        if (tile != 0)
        {
            int a = 0;
        }

        const uint32_t color = m_PPU->colors[m_PPU->palette[m_PPU->tileset[tile][y][x % 8]]];
        m_PPU->framebuffer[m_PPU->line->value][x] = color;
    }
}

} // namespace gbl
