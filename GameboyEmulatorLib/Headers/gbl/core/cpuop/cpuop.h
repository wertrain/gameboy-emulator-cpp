#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }
namespace gbl { class CPU; }
namespace gbl { class MMU; }

namespace gbl
{

class CPUOperator
{
public:
    CPUOperator(IGBLAllocator* allocator);
    ~CPUOperator();

    typedef struct OpFuncType
    {
        void (CPUOperator::*function)(CPU* cpu, MMU* mmu);
        uint8_t m;
        uint8_t t;
        uint8_t b;
    }
    OpFunc_t;

    void Initialize();

#define OPERATE(MemberName, m, t, b) void MemberName(CPU* cpu, MMU* mmu);
    #include "gbl/core/cpuop/cpuop_table.hxx"
#undef OPERATE

private:
    void DebugOpcodeError(CPU* cpu);
    void DebugInstruction(CPU* cpu, MMU* mmu, const char* disassembly, ...);
    void DebugInterruption(CPU* cpu, MMU* mmu, const char* message, ...);

private:
    IGBLAllocator* m_Allocator;
    OpFunc_t m_OpTable[0xFF];
    OpFunc_t m_OpCbTable[0xFF];
};

} // namespace gbl
