#pragma once

namespace gbl { class MMU; }
namespace gbl { class CPU; }
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
    bool BootFromMemory(void* rom, size_t romSize);
    void Shutdown();

private:
    IGBLAllocator* m_Allocator;
    MMU* m_MMU;
    CPU* m_CPU;
};

} // namespace gbl
