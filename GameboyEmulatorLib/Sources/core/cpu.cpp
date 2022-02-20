#include "pch.h"
#include "framework.h"

#include <memory>
#include "gbl/allocator.h"
#include "gbl/debug/debugger.h"
#include "gbl/core/mmu.h"
#include "gbl/core/ppu.h"
#include "gbl/core/cpuop/cpuop.h"
#include "gbl/core/cpu.h"

namespace
{

// Interruptins
constexpr uint8_t CPU_INT_VBLANK = 0x01;
constexpr uint8_t CPU_INT_LCD = (0x01 << 1);
constexpr uint8_t CPU_INT_TIMER = (0x01 << 2);
constexpr uint8_t CPU_INT_SERIAL = (0x01 << 3);
constexpr uint8_t CPU_INT_JOYPAD = (0x01 << 4);

// Check if the nth bit of a byte is one
bool UtilCheckBit(const uint8_t value, const uint8_t n)
{
    return (value & (1 << n)) != 0;
}

// Set the nth bit of the given byte to one
void UtilSetBit(uint8_t* byte, const uint8_t n)
{
    (*byte) = (*byte) | (0x01 << n);
}

// Set the nth bit of the given byte to zero
void UtilUnsetBit(uint8_t* byte, const uint8_t n)
{
    (*byte) = (*byte) & ~(0x01 << n);
}

}

namespace gbl
{

CPU::CPU(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_CPU(nullptr)
    , m_Operator(nullptr)
{
    m_CPU = static_cast<CPU_t*>(m_Allocator->Allocate(sizeof(CPU_t)));
    memset(m_CPU, 0, sizeof(CPU_t));

    m_Operator = gbl::New<CPUOperator>(m_Allocator);
}

CPU::~CPU()
{
    gbl::Delete<CPUOperator>(m_Operator, m_Allocator);

    if (m_CPU)
    {
        free(m_CPU);
        m_CPU = nullptr;
    }
}

void CPU::RunCartridge(MMU* mmu, PPU* ppu)
{
    (*mmu->GetMMU()->finishedBIOS) = true;
    m_CPU->registers.pc.word = 0x0100;
    m_CPU->registers.sp.word = 0xFFFE;
    m_CPU->registers.af.word = 0x01B0;
    m_CPU->registers.bc.word = 0x0013;
    m_CPU->registers.de.word = 0x00d8;
    m_CPU->registers.hl.word = 0x014d;

    mmu->ResetVRAM();

    ppu->EnableLCD();
}

void CPU::Tick(MMU* mmu)
{
    // 命令の読み取り
    const uint8_t op = mmu->ReadByte(m_CPU->registers.pc.word);
    m_CPU->currentOp = op << 8;
    m_CPU->currentOpAddress = m_CPU->registers.pc.word;

    // 命令の実行
    CPUOperator::OpFuncType* opfunc = m_Operator->GetOperator(op);
    m_CPU->registers.pc.word++;
    if (!opfunc || !opfunc->function)
    {
        Debugger::OpcodeError(this);
        return;
    }

    (m_Operator->*opfunc->function)(this, mmu);
    m_CPU->currentClock.m = opfunc->m;
    m_CPU->currentClock.t = opfunc->t;
    m_CPU->registers.pc.word += opfunc->b;
    
    // 割り込みのチェック
    if (m_CPU->ime && mmu->GetMMU()->intenable && (*mmu->GetMMU()->intFlags))
    {
        const uint8_t fired = mmu->GetMMU()->intenable & (*mmu->GetMMU()->intFlags);
        HandleInterrupt(mmu, fired);
    }

    m_CPU->totalClock.m += m_CPU->currentClock.m;
    m_CPU->totalClock.t += m_CPU->currentClock.t;
}

void CPU::Reset()
{
    memset(reinterpret_cast<void*>(&m_CPU->registers), 0, sizeof(m_CPU->registers));
    m_CPU->ime = false;
    m_CPU->currentClock = { 0, 0 };
    m_CPU->totalClock = { 0, 0 };
}

void CPU::EnableInterrupt()
{
    m_CPU->ime = true;
    m_CPU->currentClock = Clock_t { 1, 4 };
}

void CPU::DisableInterrupt()
{
    m_CPU->ime = false;
    m_CPU->currentClock = Clock_t{ 1, 4 };
}

void CPU::HandleInterrupt(MMU* mmu, const uint8_t in)
{
    if (in & CPU_INT_VBLANK)
    {
        HandleInterruptVBlank(mmu);
    }
}

void CPU::HandleInterruptVBlank(MMU* mmu)
{
    DebugInterruption(mmu, "VBLANK");

    m_CPU->ime = false;
    (*mmu->GetMMU()->intFlags) &= ~CPU_INT_VBLANK;

    InstructionCall(mmu, 0x0040, 0);

    m_CPU->currentClock.m += 4;
    m_CPU->currentClock.t += 16;
}

//-----------------------------------------------------------------------------

void CPU::FlagSetZero(bool value)
{
    if (value)
    {
        UtilSetBit(&m_CPU->registers.af.lo, FLAG_ZERO_BIT);
    }
    else
    {
        UtilUnsetBit(&m_CPU->registers.af.lo, FLAG_ZERO_BIT);
    }
}

void CPU::FlagSetSub(const bool value)
{
    if (value)
    {
        UtilSetBit(&m_CPU->registers.af.lo, FLAG_SUB_BIT);
    }
    else
    {
        UtilUnsetBit(&m_CPU->registers.af.lo, FLAG_SUB_BIT);
    }
}

void CPU::FlagSetHalfcarry(const bool value)
{
    if (value)
    {
        UtilSetBit(&m_CPU->registers.af.lo, FLAG_HC_BIT);
    }
    else
    {
        UtilUnsetBit(&m_CPU->registers.af.lo, FLAG_HC_BIT);
    }
}

void CPU::FlagSetCarry(bool value)
{
    if (value)
    {
        UtilSetBit(&m_CPU->registers.af.lo, FLAG_CARRY_BIT);
    }
    else
    {
        UtilUnsetBit(&m_CPU->registers.af.lo, FLAG_CARRY_BIT);
    }
}

bool CPU::Flag(const uint8_t flag)
{
    return UtilCheckBit(m_CPU->registers.af.lo, flag);
}

bool CPU::CheckCondition(const Condition condition)
{
    bool c = false;
    switch (condition)
    {
    case Condition::C:
        c = Flag(FLAG_CARRY_BIT);
        break;
    case Condition::NC:
        c = !Flag(FLAG_CARRY_BIT);
        break;
    case Condition::Z:
        c = Flag(FLAG_ZERO_BIT);
        break;
    case Condition::NZ:
        c = !Flag(FLAG_ZERO_BIT);
        break;
    case Condition::Always:
        c = true;
        break;
    }
    return c;
}

//-----------------------------------------------------------------------------

void CPU::InstructionBit(const uint8_t bit, const uint8_t bytereg)
{
    FlagSetZero(UtilCheckBit(bytereg, bit) == 0);
    FlagSetSub(false);
    FlagSetHalfcarry(true);
}

void CPU::InstructionRes(const uint8_t bit, uint8_t* value)
{
    UtilUnsetBit(value, bit);
}

void CPU::InstructionSet(const uint8_t bit, uint8_t* value)
{
    UtilSetBit(value, bit);
}

void CPU::InstructionRl(uint8_t* reg)
{
    uint8_t carry = Flag(FLAG_CARRY_BIT);
    FlagSetCarry(UtilCheckBit((*reg), 7));

    uint8_t res = ((*reg) << 1) | carry;
    FlagSetZero(res == 0);

    (*reg) = res;

    FlagSetSub(false);
    FlagSetHalfcarry(false);
}

void CPU::InstructionRlc(uint8_t* reg)
{
    FlagSetCarry(UtilCheckBit((*reg), 7));
    uint8_t carry = Flag(FLAG_CARRY_BIT);

    uint8_t res = ((*reg) << 1) | carry;
    FlagSetZero(res == 0);

    (*reg) = res;

    FlagSetSub(false);
    FlagSetHalfcarry(false);
}

void CPU::InstructionRr(uint8_t* reg)
{
    uint8_t carry = Flag(FLAG_CARRY_BIT);
    FlagSetCarry(UtilCheckBit((*reg), 0));

    uint8_t res = ((*reg) >> 1) | (carry << 7);
    FlagSetZero(res == 0);

    (*reg) = res;

    FlagSetSub(false);
    FlagSetHalfcarry(false);
}

void CPU::InstructionRrc(uint8_t* reg)
{
    FlagSetCarry(UtilCheckBit((*reg), 0));
    uint8_t carry = Flag(FLAG_CARRY_BIT);

    uint8_t res = ((*reg) >> 1) | (carry << 7);
    FlagSetZero(res == 0);

    (*reg) = res;

    FlagSetSub(false);
    FlagSetHalfcarry(false);
}

void CPU::InstructionSla(uint8_t* reg)
{
    FlagSetSub(false);
    FlagSetHalfcarry(false);

    FlagSetCarry(UtilCheckBit((*reg), 0));

    (*reg) <<= 1;

    FlagSetZero((*reg) == 0);
}

void CPU::InstructionSra(uint8_t* reg)
{
    FlagSetSub(false);
    FlagSetHalfcarry(false);

    FlagSetCarry(UtilCheckBit((*reg), 0));

    uint8_t top = UtilCheckBit((*reg), 7);
    (*reg) >>= 1;
    (*reg) |= top;

    FlagSetZero((*reg) == 0);
}

void CPU::InstructionSrl(uint8_t* reg)
{
    FlagSetSub(false);
    FlagSetHalfcarry(false);

    uint8_t top = UtilCheckBit((*reg), 7);
    (*reg) >>= 1;
    UtilUnsetBit(reg, 7);

    FlagSetCarry(top);
    FlagSetZero((*reg) == 0);
}

void CPU::InstructionSwap(uint8_t* reg)
{
    FlagSetSub(false);
    FlagSetHalfcarry(false);
    FlagSetCarry(false);

    uint8_t lower = (*reg) & 0xF;
    uint8_t higher = (*reg) >> 4;

    (*reg) = lower << 4 | higher;

    FlagSetZero((*reg) == 0);
}

void CPU::InstructionAnd(const uint8_t value)
{
    FlagSetCarry(false);
    FlagSetHalfcarry(true);
    FlagSetSub(false);

    m_CPU->registers.af.hi &= value;

    FlagSetZero(!m_CPU->registers.af.hi);
}

void CPU::InstructionXor(const uint8_t value)
{
    FlagSetCarry(false);
    FlagSetHalfcarry(false);
    FlagSetSub(false);

    m_CPU->registers.af.hi ^= value;

    FlagSetZero(!m_CPU->registers.af.hi);
}

void CPU::InstructionOr(const uint8_t value)
{
    FlagSetCarry(false);
    FlagSetHalfcarry(false);
    FlagSetSub(false);

    m_CPU->registers.af.hi |= value;

    FlagSetZero(!m_CPU->registers.af.hi);
}

void CPU::InstructionCp(const uint8_t value)
{
    FlagSetSub(true);

    FlagSetZero(m_CPU->registers.af.hi == value);
    FlagSetHalfcarry(((m_CPU->registers.af.hi & 0xf) - (value & 0xf)) < 0);
    FlagSetCarry(m_CPU->registers.af.hi < value);
}

//-----------------------------------------------------------------------------

void CPU::InstructionInc8(uint8_t* reg)
{
    FlagSetSub(false);
    FlagSetHalfcarry((((*reg) & 0xF) == 0xF));
    (*reg)++;
    FlagSetZero(((*reg) == 0));
}

void CPU::InstructionAdd8(uint8_t* reg, const uint8_t value)
{
    FlagSetSub(false);

    uint8_t res = (*reg) + value;

    FlagSetZero(res == 0);
    FlagSetCarry(((*reg) + value) > 0xFF);
    FlagSetHalfcarry((((*reg) & 0xF) + (value & 0xF)) > 0xF);

    (*reg) = res;
}

void CPU::InstructionAdd16(uint16_t* reg, const uint16_t value, const bool zero)
{
    FlagSetSub(false);

    uint16_t res = (*reg) + value;

    if (zero) FlagSetZero(res == 0);
    FlagSetCarry(((*reg) + value) > 0xFFFF);
    FlagSetHalfcarry((((*reg) & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);

    (*reg) = res;
}

void CPU::InstructionDec8(uint8_t* reg)
{
    (*reg)--;

    FlagSetSub(true);
    FlagSetHalfcarry((((*reg) & 0x0F) == 0x0F));
    FlagSetZero(((*reg) == 0));
}

void CPU::InstructionSub8(const uint8_t value)
{
    FlagSetSub(true);

    int8_t sa = static_cast<int8_t>(m_CPU->registers.af.hi - value);
    uint8_t a = static_cast<uint8_t>(sa);

    FlagSetCarry(sa < 0);
    FlagSetHalfcarry((a & 0xF) > (m_CPU->registers.af.hi & 0xF));
    FlagSetZero(a == 0);

    m_CPU->registers.af.hi = a;
}

void CPU::InstructionAdc(const uint8_t value)
{
    InstructionAdd8(&m_CPU->registers.af.hi, value + Flag(FLAG_CARRY_BIT));
}

void CPU::InstructionCall(MMU* mmu, const uint16_t addr, const uint8_t offset)
{
    m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(m_CPU->registers.sp.word, m_CPU->registers.pc.word + offset);
    m_CPU->registers.pc.word = addr;
}

void CPU::InstructionRet(MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(m_CPU->registers.sp.word);
    m_CPU->registers.sp.word += 2;
    m_CPU->registers.pc.word = addr;
}

void CPU::InstructionRetCondition(MMU* mmu, const Condition c)
{
    if (CheckCondition(c))
    {
        InstructionRet(mmu);
        m_CPU->currentClock.m += 3;
        m_CPU->currentClock.t += 12;
    }
}

void CPU::InstructionJr(const int8_t offset, const Condition c)
{
    if (CheckCondition(c))
    {
        m_CPU->registers.pc.word += offset;
        m_CPU->currentClock.m += 1;
        m_CPU->currentClock.t += 4;
    }
}

void CPU::InstructionJp(const uint16_t addr, const Condition c, const uint8_t offset)
{
    if (CheckCondition(c))
    {
        m_CPU->registers.pc.word = addr - offset;
        m_CPU->currentClock.m += 1;
        m_CPU->currentClock.t += 4;
    }
}

void CPU::DebugOpcodeError()
{

}

void CPU::DebugInstruction(MMU* mmu, const char* disassembly, ...)
{

}

void CPU::DebugInterruption(MMU* mmu, const char* message, ...)
{

}

} // namespace gbl
