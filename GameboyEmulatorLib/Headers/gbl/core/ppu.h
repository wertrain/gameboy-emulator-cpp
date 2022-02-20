#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }
namespace gbl { class MMU; }
namespace gbl { class CPU; }

namespace gbl
{

/// <summary>
/// PPU (Picture Processing Unit) class
/// </summary>
class PPU
{
public:
    enum
    {
        BUFFER_WIDTH = 160,
        BUFFER_HEIGHT = 144,
    };

public:
    PPU(IGBLAllocator* allocator);
    ~PPU();

    bool Create(MMU* mmu);
    void Destroy();

    void EnableLCD();
    bool CanRender() const;
    uint32_t GetFrameBuffer(int32_t x, int32_t y) const;
    void ResetCanRender();
    void ResetFrameBuffer();
    void Tick(CPU* cpu, MMU* mmu);

private:
    enum
    {
        PPU_MODE_OAM = 2,
        PPU_MODE_VRAM = 3,
        PPU_MODE_HBLANK = 0,
        PPU_MODE_VBLANK = 1,

        PPU_HLINES = 143,
        PPU_VLINES = 153,
    };

    typedef struct RegisterType
    {
        uint8_t value;
    }
    Register_t;

    typedef struct PPUType
    {
        uint8_t mode : 2;
        uint32_t clockCount;

        struct ControlType
        {
            union
            {
                uint8_t value;
                struct
                {
                    bool lcdDisplay : 1;
                    bool windowTilemapSelect : 1;
                    bool windowDisplay : 1;
                    bool bgWinTiledataSelect : 1;
                    bool bgTilemapSelect : 1;
                    bool spriteSize : 1;
                    bool spriteDisplay : 1;
                    bool bgWinPriority : 1;
                };
            };
        }
        *control;

        Register_t* scrollY;
        Register_t* scrollX;
        Register_t* line;
        Register_t* windowY;
        Register_t* windowX;

        uint32_t colors[4];
        uint8_t palette[4];
        uint8_t tileset[0x200][8][8];

        bool canRender;
        uint32_t framebuffer[BUFFER_HEIGHT][BUFFER_WIDTH];

        uint8_t* vram;
        uint8_t* oam;
    }
    PPU_t;

    void UpdateMemory(MMU* mmu);
    void UpdateTile(uint16_t addr, uint8_t data);
    void RenderLine(MMU* mmu);

private:
    IGBLAllocator* m_Allocator;
    PPU_t* m_PPU;
};

} // namespace gbl
