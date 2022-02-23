#include "pch.h"
#include "framework.h"

#include "gbl/core/mmu.h"
#include "gbl/core/cpu.h"
#include "gbl/core/cpuop/cpuop.h"

namespace gbl
{

CPUOperator::CPUOperator(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_OpTable()
    , m_OpCbTable()
{

}

CPUOperator::~CPUOperator()
{

}

void CPUOperator::Initialize()
{
#define OPERATE(MemberIndex, m, t, b) m_OpTable[0x##MemberIndex] = { &CPUOperator::##Op_##MemberIndex, m, t, b };
#include "gbl/core/cpuop/cpuop_table.hxx"
#undef OPERATE

    // Op_cb ‚Ì‚Ý“ÁŽê

#define OPERATE(MemberIndex, m, t, b) m_OpCbTable[0x##MemberIndex] = { &CPUOperator::##Op_Cb_##MemberIndex, m, t, b };
#include "gbl/core/cpuop/cpuop_cb_table.hxx"
#undef OPERATE
}

CPUOperator::OpFunc_t* CPUOperator::GetOperator(const uint8_t index)
{
    return &m_OpTable[index];
}

CPUOperator::OpFunc_t* CPUOperator::GetCbOperator(const uint8_t index)
{
    return &m_OpCbTable[index];
}

void CPUOperator::Op_00(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "NOP");
}

void CPUOperator::Op_01(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD BC, $%04x", word);
    cpu->m_CPU->registers.bc.word = word;
}

void CPUOperator::Op_02(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (BC), A");
    mmu->WriteByte(cpu->m_CPU->registers.bc.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_03(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC BC");
    cpu->m_CPU->registers.bc.word++;
}

void CPUOperator::Op_04(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC B");
    cpu->InstructionInc8(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_05(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC B");
    cpu->InstructionDec8(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC C");
    cpu->InstructionInc8(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_06(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD B, $%02X", byte);
    cpu->m_CPU->registers.bc.hi = byte;
}

void CPUOperator::Op_07(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLCA");
    cpu->InstructionRlc(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_08(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%02X), SP");
    mmu->WriteWord(addr, cpu->m_CPU->registers.sp.word);
}

void CPUOperator::Op_09(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, BC");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.word, false);
}

void CPUOperator::Op_0b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC BC");
    cpu->m_CPU->registers.bc.word--;
}

void CPUOperator::Op_0a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (BC)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_0d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC C");
    cpu->InstructionDec8(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD C, $%02X", byte);
    cpu->m_CPU->registers.bc.lo = byte;
}

void CPUOperator::Op_0f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRCA");
    cpu->InstructionRrc(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_11(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD DE, $%04x", word);
    cpu->m_CPU->registers.de.word = word;
}

void CPUOperator::Op_12(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (DE), A");
    mmu->WriteByte(cpu->m_CPU->registers.de.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_13(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC DE");
    cpu->m_CPU->registers.de.word++;
}

void CPUOperator::Op_14(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC D");
    cpu->InstructionInc8(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_15(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC D");
    cpu->InstructionDec8(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_16(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD D, $%02X", byte);
    cpu->m_CPU->registers.de.hi = byte;
}

void CPUOperator::Op_17(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLA");
    cpu->InstructionRl(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_18(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR $%02x", (uint8_t)offset);
    cpu->m_CPU->registers.pc.word += offset;
}

void CPUOperator::Op_19(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, DE");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.word, false);
}

void CPUOperator::Op_1a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (DE)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_1b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC DE");
    cpu->m_CPU->registers.de.word--;
}

void CPUOperator::Op_1c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC E");
    cpu->InstructionInc8(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_1d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC E");
    cpu->InstructionDec8(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_1e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD E, $%02X", byte);
    cpu->m_CPU->registers.de.lo = byte;
}

void CPUOperator::Op_1f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRA");
    cpu->InstructionRr(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_20(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR NZ, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::NZ);
}

void CPUOperator::Op_21(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD HL, $%04X", word);
    cpu->m_CPU->registers.hl.word = word;
}

void CPUOperator::Op_22(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL+), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_23(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC HL");
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_24(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC H");
    cpu->InstructionInc8(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_25(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC H");
    cpu->InstructionDec8(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_26(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD H, $%02X", byte);
    cpu->m_CPU->registers.hl.hi = byte;
}

void CPUOperator::Op_27(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DAA");
    uint8_t a = cpu->m_CPU->registers.af.hi;

    if (cpu->Flag(CPU::FLAG_SUB_BIT))
    {
        if (cpu->Flag(CPU::FLAG_HC_BIT))
        {
            a = (a - 0x06) & 0xFF;
        }
        
        if (cpu->Flag(CPU::FLAG_CARRY_BIT))
        {
            a -= 0x60;
        }
    }
    else
    {
        if (cpu->Flag(CPU::FLAG_HC_BIT) || (a & 0xF) > 9)
        {
            a += 0x06;
        }
        
        if (cpu->Flag(CPU::FLAG_CARRY_BIT) || a > 0x9F)
        {
            a += 0x60;
        }
    }

    cpu->m_CPU->registers.af.hi = a;

    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetZero(a == 0);
    cpu->FlagSetCarry(a >= 0x100);
}

void CPUOperator::Op_28(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR Z, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::Z);
}

void CPUOperator::Op_29(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, HL");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.word, false);
}

void CPUOperator::Op_2a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL+)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_2b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC HL");
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_2c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC L");
    cpu->InstructionInc8(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_2d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC L");
    cpu->InstructionDec8(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_2e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD L, $%02X", byte);
    cpu->m_CPU->registers.hl.lo = byte;
}

void CPUOperator::Op_2f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CPL");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->m_CPU->registers.af.hi = ~cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_30(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR NC, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::NC);
}

void CPUOperator::Op_31(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD SP, $%04X", word);

    cpu->m_CPU->registers.sp.word = word;
}

void CPUOperator::Op_32(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL-), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_33(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC SP");
    cpu->m_CPU->registers.sp.word++;
}

void CPUOperator::Op_34(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionInc8(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_35(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionDec8(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_36(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD (HL), $%02X", byte);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, byte);
}

void CPUOperator::Op_37(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SCF");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetCarry(true);
}

void CPUOperator::Op_38(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR C, $%2X", (uint8_t)offset);
    cpu->InstructionJr( offset, CPU::Condition::C);
}

void CPUOperator::Op_39(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, SP");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.sp.word, false);
}

void CPUOperator::Op_3a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL-)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_3b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC SP");
    cpu->m_CPU->registers.sp.word--;
}

void CPUOperator::Op_3c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC A");
    cpu->InstructionInc8(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_3d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC A");
    cpu->InstructionDec8(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_3e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, $%02X", byte);
    cpu->m_CPU->registers.af.hi = byte;
}

void CPUOperator::Op_3f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CCF");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetCarry(!cpu->Flag(CPU::FLAG_CARRY_BIT));
}


void CPUOperator::Op_40(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, B");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_41(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, C");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_42(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, D");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_43(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, E");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_44(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, H");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_45(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, L");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_46(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.bc.hi = byte;
}

void CPUOperator::Op_47(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, A");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_48(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, B");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_49(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, C");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_4a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, D");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_4b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, E");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_4c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, H");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_4d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, L");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_4e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.bc.lo = byte;
}

void CPUOperator::Op_4f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, A");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_50(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, B");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_51(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, C");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_52(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, D");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_53(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, E");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_54(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, H");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_55(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, L");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_56(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.de.hi = byte;
}

void CPUOperator::Op_57(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, A");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_58(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, B");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_59(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, C");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_5a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, D");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_5b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, E");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_5c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, H");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_5d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, L");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_5e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.de.lo = byte;
}

void CPUOperator::Op_5f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, A");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_60(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, B");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_61(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, C");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_62(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, D");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_63(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, E");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_64(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, H");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_65(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, L");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_66(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.hi = byte;
}

void CPUOperator::Op_67(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, A");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_68(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, B");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_69(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, C");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_6a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, D");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_6b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, E");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_6c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, H");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_6d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, L");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_6e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.lo = byte;
}

void CPUOperator::Op_6f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, A");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_70(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), B");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_71(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), C");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_72(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), D");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_73(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), E");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_74(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), H");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_75(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), L");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_76(CPU* cpu, MMU* mmu)
{
    // NOP
 }

void CPUOperator::Op_77(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_78(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, B");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_79(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, C");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_7a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, D");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_7b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, E");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_7c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, H");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_7d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, L");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_7e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.af.hi = byte;
}

void CPUOperator::Op_7f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, A");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_80(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, B");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_81(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, C");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_82(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, D");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_83(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, E");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_84(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, H");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_85(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, L");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_86(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADD A, (HL)");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_87(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, A");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_88(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, B");
    cpu->InstructionAdc(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_89(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, C");
    cpu->InstructionAdc(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_8a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, D");
    cpu->InstructionAdc(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_8b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, E");
    cpu->InstructionAdc(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_8c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, H");
    cpu->InstructionAdc(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_8d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, L");
    cpu->InstructionAdc(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_8e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADC A, (HL)");
    cpu->InstructionAdc(byte);
}

void CPUOperator::Op_8f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, A");
    cpu->InstructionAdc(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_90(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB B");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_91(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB C");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_92(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB D");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_93(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB E");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_94(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB H");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_95(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB L");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_96(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB (HL)");
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSub8(value);
}

void CPUOperator::Op_97(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB A");
    cpu->InstructionSub8(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_98(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, B");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_99(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, C");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, D");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, E");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, H");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, L");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, (HL)");
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSub8(value + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_9f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, A");
    cpu->InstructionSub8(cpu->m_CPU->registers.af.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_a0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND B");
    cpu->InstructionAnd(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_a1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND C");
    cpu->InstructionAnd(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_a2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND D");
    cpu->InstructionAnd(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_a3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND E");
    cpu->InstructionAnd(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_a4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND H");
    cpu->InstructionAnd(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_a5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND L");
    cpu->InstructionAnd(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_a6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "AND (HL)");
    cpu->InstructionAnd(value);
}

void CPUOperator::Op_a7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND A");
    cpu->InstructionAnd(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_a8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR B");
    cpu->InstructionXor(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_a9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR C");
    cpu->InstructionXor(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_aa(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR D");
    cpu->InstructionXor(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_ab(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR E");
    cpu->InstructionXor(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_ac(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR H");
    cpu->InstructionXor(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_ad(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR L");
    cpu->InstructionXor(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_ae(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "XOR (HL)");
    cpu->InstructionXor(value);
}

void CPUOperator::Op_af(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR A");
    cpu->InstructionXor(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_b0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR B");
    cpu->InstructionOr(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_b1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR C");
    cpu->InstructionOr(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_b2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR D");
    cpu->InstructionOr(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_b3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR E");
    cpu->InstructionOr(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_b4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR H");
    cpu->InstructionOr(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_b5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR L");
    cpu->InstructionOr(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_b6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "OR (HL)");
    cpu->InstructionOr(value);
}

void CPUOperator::Op_b7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR A");
    cpu->InstructionOr(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_b8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP B");
    cpu->InstructionCp(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_b9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP C");
    cpu->InstructionCp(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_ba(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP D");
    cpu->InstructionCp(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_bb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP E");
    cpu->InstructionCp(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_bc(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP H");
    cpu->InstructionCp(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_bd(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP L");
    cpu->InstructionCp(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_be(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "CP (HL)");
    cpu->InstructionCp(value);
}

void CPUOperator::Op_bf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP A");
    cpu->InstructionCp(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_c0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NZ");
    cpu->InstructionRetCondition(mmu, CPU::Condition::NZ);
}

void CPUOperator::Op_c1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP BC");
    cpu->m_CPU->registers.bc.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_c2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NZ, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::NZ, 0x02);
}

void CPUOperator::Op_c3(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::Always, 0x02);
}

void CPUOperator::Op_c5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH BC");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_c6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD A, $%02x", value);
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_c7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 00h");
    cpu->InstructionCall(mmu, 0x0000, 1);
}

void CPUOperator::Op_c8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET Z");
    cpu->InstructionRetCondition(mmu, CPU::Condition::Z);
}

void CPUOperator::Op_c9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET");
    cpu->InstructionRet(mmu);
}

void CPUOperator::Op_ca(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP Z, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::Z, 0x02);
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_cb(CPU* cpu, MMU* mmu)
{
    // Fetch instruction
    uint8_t op = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    cpu->m_CPU->currentOp += op;

    // Run instruction
    OpFunc_t* opfunc_cb = &m_OpCbTable[op];
    cpu->m_CPU->registers.pc.word++;
    
    if (!opfunc_cb || !opfunc_cb->function)
    {
        cpu->DebugOpcodeError();
        return;
    }

    (this->*opfunc_cb->function)(cpu, mmu);
    cpu->m_CPU->currentClock.m = opfunc_cb->m;
    cpu->m_CPU->currentClock.t = opfunc_cb->t;
    cpu->m_CPU->registers.pc.word += opfunc_cb->b;
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_cd(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CALL $%04x", addr);

    cpu->InstructionCall(mmu, addr, 2);
}

void CPUOperator::Op_cf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 08h");
    cpu->InstructionCall(mmu, 0x0008, 0);
}

void CPUOperator::Op_d0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NC");
    cpu->InstructionRetCondition(mmu, CPU::Condition::NC);
}

void CPUOperator::Op_d1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP DE");
    cpu->m_CPU->registers.de.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_d2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NC, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::NC, 0x02);
}

void CPUOperator::Op_d5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH DE");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_d7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 10h");
    cpu->InstructionCall(mmu, 0x0010, 0);
}

void CPUOperator::Op_d8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET C");
    cpu->InstructionRetCondition(mmu, CPU::Condition::C);
}

void CPUOperator::Op_d9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RETI");
    cpu->InstructionRet(mmu);
    cpu->m_CPU->ime = true;
}

void CPUOperator::Op_da(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP C, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::C, 0x02);
}

void CPUOperator::Op_df(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 18h");
    cpu->InstructionCall(mmu, 0x0018, 0);
}

void CPUOperator::Op_e0(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($FF00+$%02x), A", byte);
    uint16_t addr = 0xFF00 + byte;
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_e1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP HL");
    cpu->m_CPU->registers.hl.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_e2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD ($FF00+C), A");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_e5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH HL");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.hl.word);
}

void CPUOperator::Op_e6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "AND $%02X", value);
    cpu->InstructionAnd(value);
}

void CPUOperator::Op_e7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 20h");
    cpu->InstructionCall(mmu, 0x0020, 0);
}

void CPUOperator::Op_e8(CPU* cpu, MMU* mmu)
{
    int8_t value = (int8_t)mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD SP, $%02x", value);

    cpu->FlagSetSub(false);
    cpu->FlagSetZero(false);

    cpu->FlagSetCarry(((cpu->m_CPU->registers.sp.word) + value) > 0xFFFF);
    cpu->FlagSetHalfcarry(((cpu->m_CPU->registers.sp.word & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);

    cpu->m_CPU->registers.sp.word += value;
}

void CPUOperator::Op_e9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "JP HL");
    cpu->InstructionJp(cpu->m_CPU->registers.hl.word, CPU::Condition::Always, 0);
}

void CPUOperator::Op_ea(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%04x), A", addr);
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_ef(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 28h");
    cpu->InstructionCall(mmu, 0x0028, 0);
}

void CPUOperator::Op_f0(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($FF00+$%02x)", byte);
    uint16_t addr = 0xFF00 + byte;
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_f1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP AF");
    cpu->m_CPU->registers.af.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_f2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, ($FF00+C)");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_f3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DI");
    cpu->m_CPU->ime = false;
}

void CPUOperator::Op_f5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH AF");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.af.word);
}

void CPUOperator::Op_f7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 30h");
    cpu->InstructionCall(mmu, 0x0030, 0);
}

void CPUOperator::Op_fa(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($%04x)", addr);
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_fb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "EI");
    cpu->m_CPU->ime = true;
}

void CPUOperator::Op_fe(CPU* cpu, MMU* mmu)
{
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CP $%02x", v);
    cpu->InstructionCp(v);
}

void CPUOperator::Op_ff(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 38h");
    cpu->InstructionCall(mmu, 0x0038, 0);
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_Cb_00(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC B");
    cpu->InstructionRlc(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_01(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC C");
    cpu->InstructionRlc(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_02(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC D");
    cpu->InstructionRlc(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_03(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC E");
    cpu->InstructionRlc(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_04(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC H");
    cpu->InstructionRlc(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_05(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC L");
    cpu->InstructionRlc(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_06(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRlc(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_07(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC A");
    cpu->InstructionRlc(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_08(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC B");
    cpu->InstructionRrc(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_09(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC C");
    cpu->InstructionRrc(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_0a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC D");
    cpu->InstructionRrc(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_0b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC E");
    cpu->InstructionRrc(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_0c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC H");
    cpu->InstructionRrc(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_0d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC L");
    cpu->InstructionRrc(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_0e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRrc(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_0f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC A");
    cpu->InstructionRrc(&cpu->m_CPU->registers.af.hi);
}

// CB OP Codes

void CPUOperator::Op_Cb_10(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL B");
    cpu->InstructionRl(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_11(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL C");
    cpu->InstructionRl(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_12(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL D");
    cpu->InstructionRl(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_13(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL E");
    cpu->InstructionRl(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_14(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL H");
    cpu->InstructionRl(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_15(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL L");
    cpu->InstructionRl(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_16(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRl(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_17(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL A");
    cpu->InstructionRl(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_18(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR B");
    cpu->InstructionRr(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_19(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR C");
    cpu->InstructionRr(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_1a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR D");
    cpu->InstructionRr(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_1b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR E");
    cpu->InstructionRr(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_1c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR H");
    cpu->InstructionRr(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_1d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR L");
    cpu->InstructionRr(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_1e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRr(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_1f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR A");
    cpu->InstructionRr(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_20(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA B");
    cpu->InstructionSla(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_21(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA C");
    cpu->InstructionSla(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_22(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA D");
    cpu->InstructionSla(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_23(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA E");
    cpu->InstructionSla(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_24(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA H");
    cpu->InstructionSla(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_25(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA L");
    cpu->InstructionSla(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_26(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSla(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_27(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA A");
    cpu->InstructionSla(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_28(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA B");
    cpu->InstructionSra(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_29(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA C");
    cpu->InstructionSra(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_2a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA D");
    cpu->InstructionSra(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_2b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA E");
    cpu->InstructionSra(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_2c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA H");
    cpu->InstructionSra(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_2d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA L");
    cpu->InstructionSra(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_2e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSra(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_2f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA A");
    cpu->InstructionSra(&cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_30(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP B");
    cpu->InstructionSwap(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_31(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP C");
    cpu->InstructionSwap(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_32(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP D");
    cpu->InstructionSwap(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_33(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP E");
    cpu->InstructionSwap(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_34(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP H");
    cpu->InstructionSwap(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_35(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP L");
    cpu->InstructionSwap(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_36(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSwap(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_37(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP A");
    cpu->InstructionSwap(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_38(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL B");
    cpu->InstructionSrl(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_39(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL C");
    cpu->InstructionSrl(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_3a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL D");
    cpu->InstructionSrl(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_3b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL E");
    cpu->InstructionSrl(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_3c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL H");
    cpu->InstructionSrl(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_3d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL L");
    cpu->InstructionSrl(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_3e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSrl(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_Cb_3f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL A");
    cpu->InstructionSrl(&cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_40(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, B");
    cpu->InstructionBit(0, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_41(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, C");
    cpu->InstructionBit(0, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_42(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, D");
    cpu->InstructionBit(0, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_43(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, E");
    cpu->InstructionBit(0, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_44(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, H");
    cpu->InstructionBit(0, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_45(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, L");
    cpu->InstructionBit(0, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_46(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 0, (HL)");
    cpu->InstructionBit(0, value);
}

void CPUOperator::Op_Cb_47(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, A");
    cpu->InstructionBit(0, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_48(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, B");
    cpu->InstructionBit(1, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_49(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, C");
    cpu->InstructionBit(1, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_4a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, D");
    cpu->InstructionBit(1, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_4b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, E");
    cpu->InstructionBit(1, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_4c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, H");
    cpu->InstructionBit(1, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_4d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, L");
    cpu->InstructionBit(1, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_4e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 1, (HL)");
    cpu->InstructionBit(1, value);
}

void CPUOperator::Op_Cb_4f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, A");
    cpu->InstructionBit(1, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_50(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, B");
    cpu->InstructionBit(2, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_51(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, C");
    cpu->InstructionBit(2, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_52(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, D");
    cpu->InstructionBit(2, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_53(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, E");
    cpu->InstructionBit(2, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_54(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, H");
    cpu->InstructionBit(2, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_55(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, L");
    cpu->InstructionBit(2, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_56(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 2, (HL)");
    cpu->InstructionBit(2, value);
}

void CPUOperator::Op_Cb_57(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, A");
    cpu->InstructionBit(2, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_58(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, B");
    cpu->InstructionBit(3, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_59(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, C");
    cpu->InstructionBit(3, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_5a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, D");
    cpu->InstructionBit(3, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_5b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, E");
    cpu->InstructionBit(3, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_5c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, H");
    cpu->InstructionBit(3, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_5d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, L");
    cpu->InstructionBit(3, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_5e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 3, (HL)");
    cpu->InstructionBit(3, value);
}

void CPUOperator::Op_Cb_5f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, A");
    cpu->InstructionBit(3, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_60(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, B");
    cpu->InstructionBit(4, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_61(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, C");
    cpu->InstructionBit(4, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_62(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, D");
    cpu->InstructionBit(4, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_63(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, E");
    cpu->InstructionBit(4, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_64(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, H");
    cpu->InstructionBit(4, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_65(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, L");
    cpu->InstructionBit(4, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_66(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 4, (HL)");
    cpu->InstructionBit(4, value);
}

void CPUOperator::Op_Cb_67(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, A");
    cpu->InstructionBit(4, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_68(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, B");
    cpu->InstructionBit(5, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_69(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, C");
    cpu->InstructionBit(5, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_6a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, D");
    cpu->InstructionBit(5, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_6b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, E");
    cpu->InstructionBit(5, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_6c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, H");
    cpu->InstructionBit(5, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_6d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, L");
    cpu->InstructionBit(5, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_6e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 5, (HL)");
    cpu->InstructionBit(5, value);
}

void CPUOperator::Op_Cb_6f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, A");
    cpu->InstructionBit(5, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_70(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, B");
    cpu->InstructionBit(6, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_71(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, C");
    cpu->InstructionBit(6, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_72(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, D");
    cpu->InstructionBit(6, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_73(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, E");
    cpu->InstructionBit(6, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_74(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, H");
    cpu->InstructionBit(6, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_75(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, L");
    cpu->InstructionBit(6, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_76(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 6, (HL)");
    cpu->InstructionBit(6, value);
}

void CPUOperator::Op_Cb_77(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, A");
    cpu->InstructionBit(6, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_78(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, B");
    cpu->InstructionBit(7, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_79(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, C");
    cpu->InstructionBit(7, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_7a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, D");
    cpu->InstructionBit(7, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_7b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, E");
    cpu->InstructionBit(7, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_7c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, H");
    cpu->InstructionBit(7, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_7d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, L");
    cpu->InstructionBit(7, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_7e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 7, (HL)");
    cpu->InstructionBit(7, value);
}

void CPUOperator::Op_Cb_7f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, A");
    cpu->InstructionBit(7, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_80(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, B");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_81(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, C");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_82(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, D");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_83(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, E");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_84(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, H");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_85(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, L");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_86(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 0, (HL)");
    cpu->InstructionRes(0, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_87(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, A");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_88(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, B");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_89(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, C");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_8a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, D");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_8b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, E");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_8c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, H");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_8d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, L");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_8e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 1, (HL)");
    cpu->InstructionRes(1, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_8f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, A");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_90(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, B");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_91(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, C");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_92(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, D");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_93(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, E");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_94(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, H");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_95(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, L");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_96(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 2, (HL)");
    cpu->InstructionRes(2, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_97(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, A");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_98(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, B");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_99(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, C");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_9a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, D");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_9b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, E");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_9c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, H");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_9d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, L");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_9e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 3, (HL)");
    cpu->InstructionRes(3, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_9f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, A");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.af.hi);
}


void CPUOperator::Op_Cb_a0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, B");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_a1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, C");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_a2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, D");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_a3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, E");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_a4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, H");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_a5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, L");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_a6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 4, (HL)");
    cpu->InstructionRes(4, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_a7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, A");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_a8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, B");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_a9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, C");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_aa(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, D");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_ab(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, E");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_ac(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, H");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_ad(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, L");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_ae(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 5, (HL)");
    cpu->InstructionRes(5, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_af(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, A");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.af.hi);
}



void CPUOperator::Op_Cb_b0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, B");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_b1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, C");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_b2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, D");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_b3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, E");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_b4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, H");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_b5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, L");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_b6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 6, (HL)");
    cpu->InstructionRes(6, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_b7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, A");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_Cb_b8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, B");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_Cb_b9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, C");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_Cb_ba(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, D");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_Cb_bb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, E");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_Cb_bc(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, H");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_Cb_bd(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, L");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_Cb_be(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 7, (HL)");
    cpu->InstructionRes(7, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::Op_Cb_bf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, A");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.af.hi);
}

//-----------------------------------------------------------------------------

void CPUOperator::DebugOpcodeError(CPU* cpu)
{

}

void CPUOperator::DebugInstruction(CPU* cpu, MMU* mmu, const char* disassembly, ...)
{

}

void CPUOperator::DebugInterruption(CPU* cpu, MMU* mmu, const char* message, ...)
{

}

} // namespace gbl
