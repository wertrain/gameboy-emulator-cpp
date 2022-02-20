#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }
namespace gbl { class Cartridge; }

namespace gbl
{

class MMU
{
public:
    typedef struct MemoryManagementUnit
    {
        uint8_t bios[0x100];
        struct
        {
            union
            {
                uint8_t address[0x10000];

                struct
                {
                    uint8_t rom[2][0x4000];
                    uint8_t vram[0x2000];
                    uint8_t eram[0x2000];
                    uint8_t wram[0x2000];
                    uint8_t wrams[0x1E00];
                    uint8_t oam[0xA0];
                    uint8_t empty[0x60];
                    uint8_t io[0x40];
                    uint8_t ppu[0x40];
                    uint8_t zram[0x80];
                    uint8_t intenable;
                };
            };
        };
        uint8_t* finishedBIOS;

        uint8_t* joyFlags;
        uint8_t* intFlags;

        struct
        {
            bool empty : 1;
            uint16_t address;
            union
            {
                uint16_t d16;
                uint8_t d8;
            };
        }
        lastWritten;
    }
    MMU_t;

public:
    MMU(IGBLAllocator* allocator);
    ~MMU();

    void LoadBIOS();
    bool IsFinishedBIOS() const;
    void LoadCartridge(const Cartridge* cartridge);
    void ResetVRAM();

    uint8_t ReadByte(uint16_t address);
    uint16_t ReadWord(uint16_t address);
    uint8_t ReadAddress8(uint16_t address);
    uint16_t ReadAddress16(uint16_t address);

    uint8_t WriteByte(uint16_t address, uint8_t data);
    uint8_t WriteWord(uint16_t address, uint16_t data);
    void WriteAddress8(uint16_t address, uint8_t data);
    void WriteAddress16(uint16_t address, uint16_t data);

    MMU_t* GetMMU() const { return m_MMU; }

private:
    IGBLAllocator* m_Allocator;
    MMU_t* m_MMU;
};

} // namespace gbl
