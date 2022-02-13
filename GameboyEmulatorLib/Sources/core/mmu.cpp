#include "pch.h"
#include "framework.h"

#include <memory>
#include "gbl/allocator.h"
#include "gbl/core/mmu.h"

namespace
{

static uint8_t BIOS[0x100] =
{
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

}

namespace gbl
{

MMU::MMU(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_MMU(nullptr)
{
    m_MMU = static_cast<MMU_t*>(m_Allocator->Allocate(sizeof(MMU_t)));
    memset(m_MMU, 0, sizeof(MMU_t));
    m_MMU->finishedBIOS = m_MMU->address + 0xFF50;
}

MMU::~MMU()
{
    if (m_MMU)
    {
        m_Allocator->Free(m_MMU);
        m_MMU = nullptr;
    }
}

void MMU::LoadBIOS()
{
    memcpy((void*)m_MMU->bios, (const void*)BIOS, sizeof(BIOS));
    (*m_MMU->finishedBIOS) = false;
}

bool MMU::IsFinishedBIOS() const
{
    return (*m_MMU->finishedBIOS) != 0;
}

uint8_t MMU::ReadByte(const uint16_t address)
{
    if (!(*m_MMU->finishedBIOS) && address >= 0x00 && address <= 0xFF)
    {
        return m_MMU->bios[address];
    }

    // Read fragmented memory
    switch ((address & 0xF000) >> 12)
    {
    // ROM BANK 0
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
        return m_MMU->rom[0][address];

    // ROM BANK 1
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
        return m_MMU->rom[1][address - 0x4000];

    // Video RAM
    case 0x8:
    case 0x9:
        return m_MMU->vram[address - 0x8000];

    // External RAM
    case 0xA:
    case 0xB:
        return m_MMU->eram[address - 0xA000];

    // Working RAM
    case 0xC:
    case 0xD:
        return m_MMU->wram[address - 0xC000];

    // Working RAM Shadow
    case 0xE:
        return m_MMU->wram[address - 0xC000];

    case 0xF:
        switch (address & 0x0F00)
        {
        // More Working RAM Shadow
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            return m_MMU->wram[address - 0x1FFF];

        // Sprite information
        case 0xE00:
            if (address < 0xFEA0)
            {
                return m_MMU->oam[address & 0xFF];
            }
            else
            {
                return 0;
            }

        case 0xF00:
            if (address == 0xFFFF)
            {
                return m_MMU->intenable;
            }
            else
            {
                // Memory-mapped IO
                switch (address & 0x00F0)
                {
                case 0x00:
                    return m_MMU->io[address & 0xFF];

                // PPU
                case 0x40: case 0x50: case 0x60: case 0x70:
                    return m_MMU->ppu[address - 0xFF40];

                case 0x80: case 0x90: case 0xA0: case 0xB0:
                case 0xC0: case 0xD0: case 0xE0: case 0xF0:
                    return m_MMU->zram[address & 0x7F];
                }
            }
        }
    }
    _ASSERT(false);
    return 0;
}

uint16_t MMU::ReadWord(const uint16_t address)
{
    return (ReadByte(address) | (ReadByte(address + 1) << 8));
}

uint8_t MMU::ReadAddress8(const uint16_t address)
{
    if (!(*m_MMU->finishedBIOS) && address >= 0x00 && address <= 0xFF)
    {
        return m_MMU->bios[address];
    }

    return *(m_MMU->address + address);
}

uint16_t MMU::ReadAddress16(const uint16_t address)
{
    return *(reinterpret_cast<uint16_t*>(m_MMU->address + address));
}

uint8_t MMU::WriteByte(const uint16_t address, const uint8_t data)
{
    if (!(*m_MMU->finishedBIOS) && address >= 0x00 && address <= 0xFF)
    {
        return 0;
    }

    m_MMU->lastWritten.empty = false;
    m_MMU->lastWritten.address = address;
    m_MMU->lastWritten.d8 = data;

    switch ((address & 0xF000) >> 12)
    {
        // ROM BANKS are read only
    case 0x0: case 0x1: case 0x2: case 0x3:
    case 0x4: case 0x5: case 0x6: case 0x7:
        return 0;

    // Video RAM
    case 0x8:
    case 0x9:
        m_MMU->vram[address - 0x8000] = data;
        return 1;

    // External RAM
    case 0xA:
    case 0xB:
        m_MMU->eram[address - 0xA000] = data;
        return 2;

    // Working RAM
    case 0xC:
    case 0xD:
        m_MMU->wram[address - 0xC000] = data;
        return 3;

    // Working RAM Shadow
    case 0xE:
        m_MMU->wram[address - 0xC000] = data;
        return 4;

    case 0xF:
        switch (address & 0x0F00)
        {
        // More Working RAM Shadow
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            m_MMU->wram[address - 0x1FFF] = data;
            return 4;

        // Sprite information
        case 0xE00:
            if (address < 0xFEA0)
            {
                m_MMU->oam[address & 0xFF] = data;
                return 5;
            }
            else
            {
                return 0;
            }

        case 0xF00:
            if (address == 0xFFFF)
            {
                m_MMU->intenable = data;
                return 0xA;
            }
            else
            {
                // Memory-mapped IO
                switch (address & 0x00F0)
                {
                case 0x00:
                    m_MMU->io[address & 0xFF] = data;
                    return 6;

                // PPU
                case 0x40: case 0x50: case 0x60: case 0x70:
                    m_MMU->ppu[address - 0xFF40] = data;
                    return 7;

                case 0x80: case 0x90: case 0xA0: case 0xB0:
                case 0xC0: case 0xD0: case 0xE0: case 0xF0:
                    m_MMU->zram[address & 0x7F] = data;
                    return 8;
                }
            }
        }
    }
}

uint8_t MMU::WriteWord(const uint16_t address, const uint8_t data)
{
    uint8_t r = WriteByte(address, data & 0xFF) & WriteByte(address + 1, data >> 8);
    m_MMU->lastWritten.empty = false;
    m_MMU->lastWritten.address = address;
    m_MMU->lastWritten.d16 = data;
    return r;
}

void MMU::WriteAddress8(const uint16_t address, const uint8_t data)
{
    m_MMU->address[address] = data;
}

void MMU::WriteAddress16(const uint16_t address, const uint16_t data)
{
    m_MMU->lastWritten.empty = false;
    m_MMU->lastWritten.address = address;
    m_MMU->lastWritten.d16 = data;
    uint16_t* pos = (reinterpret_cast<uint16_t*>(m_MMU->address + address));
    *pos = data;
}

} // namespace gbl
