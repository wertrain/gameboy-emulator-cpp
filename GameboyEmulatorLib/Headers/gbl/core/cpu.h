#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }
namespace gbl { class PPU; }
namespace gbl { class MMU; }
namespace gbl { class CPUOperator; }

namespace gbl
{

/// <summary>
/// CPU (Central Processing Unit) class
/// </summary>
class CPU
{
public:
    // Interruptins
    enum
    {
        CPU_INT_VBLANK = 0x01,
        CPU_INT_LCD = (0x01 << 1),
        CPU_INT_TIMER = (0x01 << 2),
        CPU_INT_SERIAL = (0x01 << 3),
        CPU_INT_JOYPAD = (0x01 << 4),
    };

public:
    CPU(IGBLAllocator* allocator);
    ~CPU();

    void RunCartridge(MMU* mmu, PPU* ppu);
    void Tick(MMU* mmu);
    void Reset();

    uint32_t GetCurrentTick() const { return m_CPU->currentClock.t; }

private:
    typedef union Register16Type
    {
        struct
        {
            uint8_t lo;
            uint8_t hi;
        };
        uint16_t word;
    }
    Reg16_t;

    typedef struct ClockType
    {
        uint32_t m;
        uint32_t t;
    }
    Clock_t;

    typedef struct CPUType
    {
        Clock_t currentClock;
        Clock_t totalClock;

        struct
        {
            Reg16_t af, bc, de, hl;
            Reg16_t sp, pc;
        }
        registers;

        bool ime;

        uint16_t currentOp;
        uint16_t currentOpAddress;
    }
    CPU_t;

    typedef struct OpFuncType
    {
        void (*function)(MMU* mmu);
        uint8_t m;
        uint8_t t;
        uint8_t b;
    }
    OpFunc_t;

    enum
    {
        FLAG_ZERO_BIT = 7,
        FLAG_SUB_BIT = 6,
        FLAG_HC_BIT = 5,
        FLAG_CARRY_BIT = 4,
    };

private:
    enum class Condition : uint8_t
    {
        C,
        NC,
        Z,
        NZ,
        Always
    };

    void EnableInterrupt();
    void DisableInterrupt();
    void HandleInterrupt(MMU* mmu, uint8_t in);
    void HandleInterruptVBlank(MMU* mmu);

    inline void FlagSetZero(bool value);
    inline void FlagSetSub(bool value);
    inline void FlagSetHalfcarry(bool value);
    inline void FlagSetCarry(bool value);
    inline bool Flag(const uint8_t flag);
    inline bool CheckCondition(Condition condition);

    void InstructionBit(uint8_t bit, uint8_t bytereg);
    void InstructionRes(uint8_t bit, uint8_t* value);
    void InstructionSet(uint8_t bit, uint8_t* value);
    void InstructionRl(uint8_t* reg);
    void InstructionRlc(uint8_t* reg);
    void InstructionRr(uint8_t* reg);
    void InstructionRrc(uint8_t* reg);
    void InstructionSla(uint8_t* reg);
    void InstructionSra(uint8_t* reg);
    void InstructionSrl(uint8_t* reg);
    void InstructionSwap(uint8_t* reg);
    void InstructionAnd(uint8_t value);
    void InstructionXor(uint8_t value);
    void InstructionOr(uint8_t value);
    void InstructionCp(uint8_t value);

    void InstructionInc8(uint8_t* reg);
    void InstructionAdd8(uint8_t* reg, const uint8_t value);
    void InstructionAdd16(uint16_t* reg, const uint16_t value, bool zero);
    void InstructionDec8(uint8_t* reg);
    void InstructionSub8(uint8_t value);
    void InstructionAdc(uint8_t value);

    void InstructionCall(MMU* mmu, uint16_t addr, uint8_t offset);
    void InstructionRet(MMU* mmu);
    void InstructionRetCondition(MMU* mmu, Condition c);
    void InstructionJr(int8_t offset, Condition c);
    void InstructionJp(uint16_t addr, Condition c, uint8_t offset);

    void DebugOpcodeError();
    void DebugInstruction(MMU* mmu, const char* disassembly, ...);
    void DebugInterruption(MMU* mmu, const char* message, ...);

private:
    IGBLAllocator* m_Allocator;
    CPU_t* m_CPU;
    CPUOperator* m_Operator;

    friend class CPUOperator;
};

} // namespace gbl
