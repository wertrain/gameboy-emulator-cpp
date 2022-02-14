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
    int opTableIndex = 0x00;
#define OPERATE(MemberName, m, t, b) m_OpTable[opTableIndex++] = { &CPUOperator::MemberName, m, t, b };
    #include "gbl/core/cpuop/cpuop_table.hxx"
#undef OPERATE
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

#if 0
void CPUOperator::Op_05(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC B");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC C");
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_06(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD B, $%02X", byte);
    cpu->m_CPU->registers.bc.hi = byte;
}

void CPUOperator::Op_07(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLCA");
    cpu_ins_rlc(cpu, &cpu->m_CPU->registers.af.hi);
    cpu_flag_set_zero(cpu, false);
}

void CPUOperator::Op_08(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%02X), SP");
    mmu_write_word(mmu, addr, cpu->m_CPU->registers.sp.word);
}

void CPUOperator::Op_09(cpu_t* cpu, mmu_t* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, BC");
    cpu_ins_add16(cpu, &cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.word, false);
}

void CPUOperator::Op_0b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC BC");
    cpu->m_CPU->registers.bc.word--;
}

void CPUOperator::Op_0a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (BC)");
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_0d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC C");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD C, $%02X", byte);
    cpu->m_CPU->registers.bc.lo = byte;
}

void CPUOperator::Op_0f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRCA");
    cpu_ins_rrc(cpu, &cpu->m_CPU->registers.af.hi);
    cpu_flag_set_zero(cpu, false);
}


void CPUOperator::Op_11(cpu_t* cpu, mmu_t* mmu)
{
    uint16_t word = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD DE, $%04x", word);
    cpu->m_CPU->registers.de.word = word;
}

void CPUOperator::Op_12(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (DE), A");
    mmu_write_byte(mmu, cpu->m_CPU->registers.de.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_13(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC DE");
    cpu->m_CPU->registers.de.word++;
}

void CPUOperator::Op_14(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC D");
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_15(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC D");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_16(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD D, $%02X", byte);
    cpu->m_CPU->registers.de.hi = byte;
}

void CPUOperator::Op_17(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLA");
    cpu_ins_rl(cpu, &cpu->m_CPU->registers.af.hi);
    cpu_flag_set_zero(cpu, false);
}

void CPUOperator::Op_18(CPU* cpu, MMU* mmu)
{
    int8_t offset = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JR $%02x", (uint8_t)offset);
    cpu->m_CPU->registers.pc.word += offset;
}

void CPUOperator::Op_19(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, DE");
    cpu_ins_add16(cpu, &cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.word, false);
}

void CPUOperator::Op_1a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (DE)");
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_1b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC DE");
    cpu->m_CPU->registers.de.word--;
}

void CPUOperator::Op_1c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC E");
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_1d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC E");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_1e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD E, $%02X", byte);
    cpu->m_CPU->registers.de.lo = byte;
}

void CPUOperator::Op_1f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRA");
    cpu_ins_rr(cpu, &cpu->m_CPU->registers.af.hi);
    cpu_flag_set_zero(cpu, false);
}

void CPUOperator::Op_20(cpu_t* cpu, mmu_t* mmu)
{
    int8_t offset = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JR NZ, $%2X", (uint8_t)offset);
    cpu_ins_jr(cpu, offset, CPU_CONDITION_NZ);
}

void CPUOperator::Op_21(cpu_t* cpu, mmu_t* mmu)
{
    uint16_t word = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD HL, $%04X", word);
    cpu->m_CPU->registers.hl.word = word;
}

void CPUOperator::Op_22(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL+), A");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
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
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_25(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC H");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_26(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD H, $%02X", byte);
    cpu->m_CPU->registers.hl.hi = byte;
}

void CPUOperator::Op_27(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DAA");
    uint8_t a = cpu->m_CPU->registers.af.hi;

    if (cpu_flag(cpu, CPU_FLAG_SUB_BIT))
    {
        if (cpu_flag(cpu, CPU_FLAG_HC_BIT))
            a = (a - 0x06) & 0xFF;
        if (cpu_flag(cpu, CPU_FLAG_CARRY_BIT))
            a -= 0x60;
    }
    else
    {
        if (cpu_flag(cpu, CPU_FLAG_HC_BIT) || (a & 0xF) > 9)
            a += 0x06;
        if (cpu_flag(cpu, CPU_FLAG_CARRY_BIT) || a > 0x9F)
            a += 0x60;
    }

    cpu->m_CPU->registers.af.hi = a;

    cpu_flag_set_halfcarry(cpu, false);
    cpu_flag_set_zero(cpu, a == 0);
    cpu_flag_set_carry(cpu, a >= 0x100);
}

void CPUOperator::Op_28(CPU* cpu, MMU* mmu)
{
    int8_t offset = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JR Z, $%2X", (uint8_t)offset);
    cpu_ins_jr(cpu, offset, CPU_CONDITION_Z);
}

void CPUOperator::Op_29(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, HL");
    cpu_ins_add16(cpu, &cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.word, false);
}

void CPUOperator::Op_2a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL+)");
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_2d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC L");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_2e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD L, $%02X", byte);
    cpu->m_CPU->registers.hl.lo = byte;
}

void CPUOperator::Op_2f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CPL");
    cpu_flag_set_sub(cpu, false);
    cpu_flag_set_halfcarry(cpu, false);
    cpu->m_CPU->registers.af.hi = ~cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_30(CPU* cpu, MMU* mmu)
{
    int8_t offset = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JR NC, $%2X", (uint8_t)offset);
    cpu_ins_jr(cpu, offset, CPU_CONDITION_NC);
}

void CPUOperator::Op_31(cpu_t* cpu, mmu_t* mmu)
{
    uint16_t word = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD SP, $%04X", word);

    cpu->m_CPU->registers.sp.word = word;
}

void CPUOperator::Op_32(cpu_t* cpu, mmu_t* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL-), A");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
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
    uint8_t v = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    cpu_ins_inc8(cpu, &v);
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_35(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC (HL)");
    uint8_t v = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    cpu_ins_dec8(cpu, &v);
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_36(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD (HL), $%02X", byte);
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, byte);
}

void CPUOperator::Op_37(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SCF");
    cpu_flag_set_sub(cpu, false);
    cpu_flag_set_halfcarry(cpu, false);
    cpu_flag_set_carry(cpu, true);
}

void CPUOperator::Op_38(CPU* cpu, MMU* mmu)
{
    int8_t offset = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JR C, $%2X", (uint8_t)offset);
    cpu_ins_jr(cpu, offset, CPU_CONDITION_C);
}

void CPUOperator::Op_39(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, SP");
    cpu_ins_add16(cpu, &cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.sp.word, false);
}

void CPUOperator::Op_3a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL-)");
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    cpu_ins_inc8(cpu, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_3d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC A");
    cpu_ins_dec8(cpu, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_3e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, $%02X", byte);
    cpu->m_CPU->registers.af.hi = byte;
}

void CPUOperator::Op_3f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CCF");
    cpu_flag_set_sub(cpu, false);
    cpu_flag_set_halfcarry(cpu, false);
    cpu_flag_set_carry(cpu, !cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_71(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), C");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_72(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), D");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_73(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), E");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_74(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), H");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_75(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), L");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_77(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), A");
    mmu_write_byte(mmu, cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
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
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
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
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_81(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, C");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_82(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, D");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_83(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, E");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_84(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, H");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_85(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, L");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_86(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADD A, (HL)");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_87(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, A");
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_88(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, B");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_89(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, C");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_8a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, D");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_8b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, E");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_8c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, H");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_8d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, L");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_8e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADC A, (HL)");
    cpu_ins_adc(cpu, byte);
}

void CPUOperator::Op_8f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, A");
    cpu_ins_adc(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_90(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB B");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_91(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB C");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_92(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB D");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_93(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB E");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_94(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB H");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_95(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB L");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_96(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB (HL)");
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    cpu_ins_sub8(cpu, value);
}

void CPUOperator::Op_97(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB A");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_98(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, B");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.bc.hi + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_99(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, C");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.bc.lo + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, D");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.de.hi + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, E");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.de.lo + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, H");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.hl.hi + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, L");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.hl.lo + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, (HL)");
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    cpu_ins_sub8(cpu, value + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_9f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, A");
    cpu_ins_sub8(cpu, cpu->m_CPU->registers.af.hi + cpu_flag(cpu, CPU_FLAG_CARRY_BIT));
}

void CPUOperator::Op_a0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND B");
    cpu_ins_and(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_a1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND C");
    cpu_ins_and(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_a2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND D");
    cpu_ins_and(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_a3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND E");
    cpu_ins_and(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_a4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND H");
    cpu_ins_and(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_a5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND L");
    cpu_ins_and(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_a6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "AND (HL)");
    cpu_ins_and(cpu, value);
}

void CPUOperator::Op_a7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND A");
    cpu_ins_and(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_a8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR B");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_a9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR C");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_aa(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR D");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_ab(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR E");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_ac(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR H");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_ad(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR L");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_ae(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "XOR (HL)");
    cpu_ins_xor(cpu, value);
}

void CPUOperator::Op_af(cpu_t* cpu, mmu_t* mmu)
{
    DebugInstruction(cpu, mmu, "XOR A");
    cpu_ins_xor(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_b0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR B");
    cpu_ins_or(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_b1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR C");
    cpu_ins_or(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_b2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR D");
    cpu_ins_or(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_b3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR E");
    cpu_ins_or(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_b4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR H");
    cpu_ins_or(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_b5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR L");
    cpu_ins_or(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_b6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "OR (HL)");
    cpu_ins_or(cpu, value);
}

void CPUOperator::Op_b7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR A");
    cpu_ins_or(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_b8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP B");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_b9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP C");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_ba(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP D");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_bb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP E");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_bc(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP H");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_bd(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP L");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_be(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "CP (HL)");
    cpu_ins_cp(cpu, value);
}

void CPUOperator::Op_bf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP A");
    cpu_ins_cp(cpu, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_c0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NZ");
    cpu_ins_ret_condition(cpu, mmu, CPU_CONDITION_NZ);
}

void CPUOperator::Op_c1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP BC");
    cpu->m_CPU->registers.bc.word = mmu_read_word(mmu, cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_c2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NZ, $%04x", addr);
    cpu_ins_jp(cpu, addr, CPU_CONDITION_NZ, 0x02);
}

void CPUOperator::Op_c3(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP $%04x", addr);
    cpu_ins_jp(cpu, addr, CPU_CONDITION_ALWAYS, 0x02);
}

void CPUOperator::Op_c5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH BC");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu_write_word(mmu, cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_c6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD A, $%02x", value);
    cpu_ins_add8(cpu, &cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_c7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 00h");
    cpu_ins_call(cpu, mmu, 0x0000, 1);
}

void CPUOperator::Op_c8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET Z");
    cpu_ins_ret_condition(cpu, mmu, CPU_CONDITION_Z);
}

void CPUOperator::Op_c9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET");
    cpu_ins_ret(cpu, mmu);
}

void CPUOperator::Op_ca(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP Z, $%04x", addr);
    cpu_ins_jp(cpu, addr, CPU_CONDITION_Z, 0x02);
}

void CPUOperator::Op_cd(cpu_t* cpu, mmu_t* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CALL $%04x", addr);

    cpu_ins_call(cpu, mmu, addr, 2);
}

void CPUOperator::Op_cf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 08h");
    cpu_ins_call(cpu, mmu, 0x0008, 0);
}

void CPUOperator::Op_d0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NC");
    cpu_ins_ret_condition(cpu, mmu, CPU_CONDITION_NC);
}

void CPUOperator::Op_d1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP DE");
    cpu->m_CPU->registers.de.word = mmu_read_word(mmu, cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_d2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NC, $%04x", addr);
    cpu_ins_jp(cpu, addr, CPU_CONDITION_NC, 0x02);
}

void CPUOperator::Op_d5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH DE");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu_write_word(mmu, cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_d7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 10h");
    cpu_ins_call(cpu, mmu, 0x0010, 0);
}

void CPUOperator::Op_d8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET C");
    cpu_ins_ret_condition(cpu, mmu, CPU_CONDITION_C);
}

void CPUOperator::Op_d9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RETI");
    cpu_ins_ret(cpu, mmu);
    cpu->ime = true;
}

void CPUOperator::Op_da(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP C, $%04x", addr);
    cpu_ins_jp(cpu, addr, CPU_CONDITION_C, 0x02);
}

void CPUOperator::Op_df(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 18h");
    cpu_ins_call(cpu, mmu, 0x0018, 0);
}

void CPUOperator::Op_e0(cpu_t* cpu, mmu_t* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($FF00+$%02x), A", byte);
    uint16_t addr = 0xFF00 + byte;
    mmu_write_byte(mmu, addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_e1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP HL");
    cpu->m_CPU->registers.hl.word = mmu_read_word(mmu, cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_e2(cpu_t* cpu, mmu_t* mmu)
{
    DebugInstruction(cpu, mmu, "LD ($FF00+C), A");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    mmu_write_byte(mmu, addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_e5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH HL");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu_write_word(mmu, cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.hl.word);
}

void CPUOperator::Op_e6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "AND $%02X", value);
    cpu_ins_and(cpu, value);
}

void CPUOperator::Op_e7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 20h");
    cpu_ins_call(cpu, mmu, 0x0020, 0);
}

void CPUOperator::Op_e8(CPU* cpu, MMU* mmu)
{
    int8_t value = (int8_t)mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD SP, $%02x", value);

    cpu_flag_set_sub(cpu, false);
    cpu_flag_set_zero(cpu, false);

    cpu_flag_set_carry(cpu, ((cpu->m_CPU->registers.sp.word) + value) > 0xFFFF);
    cpu_flag_set_halfcarry(cpu, ((cpu->m_CPU->registers.sp.word & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);

    cpu->m_CPU->registers.sp.word += value;
}

void CPUOperator::Op_e9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "JP HL");
    cpu_ins_jp(cpu, cpu->m_CPU->registers.hl.word, CPU_CONDITION_ALWAYS, 0);
}

void CPUOperator::Op_ea(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%04x), A", addr);
    mmu_write_byte(mmu, addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_ef(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 28h");
    cpu_ins_call(cpu, mmu, 0x0028, 0);
}

void CPUOperator::Op_f0(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($FF00+$%02x)", byte);
    uint16_t addr = 0xFF00 + byte;
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, addr);
}

void CPUOperator::Op_f1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP AF");
    cpu->m_CPU->registers.af.word = mmu_read_word(mmu, cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_f2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, ($FF00+C)");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, addr);
}

void CPUOperator::Op_f3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DI");
    cpu->ime = false;
}

void CPUOperator::Op_f5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH AF");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu_write_word(mmu, cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.af.word);
}

void CPUOperator::Op_f7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 30h");
    cpu_ins_call(cpu, mmu, 0x0030, 0);
}

void CPUOperator::Op_fa(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu_read_word(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($%04x)", addr);
    cpu->m_CPU->registers.af.hi = mmu_read_byte(mmu, addr);
}

void CPUOperator::Op_fb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "EI");
    cpu->ime = true;
}

void CPUOperator::Op_fe(CPU* cpu, MMU* mmu)
{
    uint8_t v = mmu_read_byte(mmu, cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CP $%02x", v);
    cpu_ins_cp(cpu, v);
}

void CPUOperator::Op_ff(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 38h");
    cpu_ins_call(cpu, mmu, 0x0038, 0);
}

#endif

} // namespace gbl
