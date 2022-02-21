#pragma once

namespace gbl { class MMU; }
namespace gbl { class PPU; }
namespace gbl { class CPU; }
namespace gbl { class Cartridge; }
namespace gbl { class ILCD; }
namespace gbl { class IGBLAllocator; }

namespace gbl
{

class GameBoyEmulator
{
public:
    GameBoyEmulator(IGBLAllocator* allocator);
    GameBoyEmulator();
    ~GameBoyEmulator();

    bool Boot(const char* romFilePath);
    bool BootFromCartridge(const Cartridge* cartridge);
    void Shutdown();
    void SetLCD(ILCD* lcd);
    bool IsRunning() const;

private:
    void Run();
    void UpdateLCD();

private:
    IGBLAllocator* m_Allocator;
    MMU* m_MMU;
    PPU* m_PPU;
    CPU* m_CPU;
    ILCD* m_LCD;
    bool m_IsRunning;
};

} // namespace gbl
