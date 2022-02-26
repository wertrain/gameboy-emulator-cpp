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

    OpFunc_t* GetOperator(uint8_t index);
    OpFunc_t* GetCbOperator(uint8_t index);

#define OPCODE(address, mnemonic, length, cycles0, cycles1, group, flagZ, flagN, flagH, flagC, operand1, operand2, count) void Op_##address(CPU* cpu, MMU* mmu);
    #include "gbl/core/cpuop/cpuop_table.hxx"
#undef OPCODE

#define OPCODE(address, mnemonic, length, cycles0, cycles1, group, flagZ, flagN, flagH, flagC, operand1, operand2, count) void OpCb_##address(CPU* cpu, MMU* mmu);
    #include "gbl/core/cpuop/cpuop_cb_table.hxx"
#undef OPCODE

private:
    void DebugOpcodeError(CPU* cpu);
    void DebugInstruction(CPU* cpu, MMU* mmu, const char* disassembly, ...);
    void DebugInterruption(CPU* cpu, MMU* mmu, const char* message, ...);

private:
    IGBLAllocator* m_Allocator;
    OpFunc_t m_OpTable[0xFF + 1];
    OpFunc_t m_OpCbTable[0xFF + 1];

    friend class CPU;
};

} // namespace gbl
