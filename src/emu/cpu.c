#include "moses/moses.h"



/* executes group 0b01 instructions */
static void ex_group01(NES *nes, Instr01 instruction) {
    // some function to parse operand value
    uint8_t operand = 0; // function here
    uint8_t result;
    // switch case and perform operation
    switch (instruction.op) {
        case ORA: {
            result = nes->cpu.a | operand;
            // set flag
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result,7));    // N
            // set accumulator
            nes->cpu.a = result;
            break;
        }
        case AND: {
            result = nes->cpu.a & operand;
            // set flags
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result, 7));   // N

            nes->cpu.a = result;
            break;
        }
        case EOR: {
            result = nes->cpu.a ^ operand;
            // set flags
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result, 7));   // N

            nes->cpu.a = result;
            break;
        }
        case ADC: {
            result = nes->cpu.a + operand + BIT(nes->cpu.p, 0);
            // set flags
            nes->cpu.p = ORBS(nes->cpu.p, 0, result > 0xff);    // C
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 6, (result ^ nes->cpu.a) & (result ^ operand) & 0x80); // V
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result, 7));   // N

            nes->cpu.a = result;
            break;
        }
        case STA: {
            // store accumulator val in memory (operand = mem. loc.)
            write_addr(nes, operand, nes->cpu.a);
            break;
        }
        case LDA: {
            // load mem. val. (operand) into accumulator
            // set flags
            nes->cpu.p = ORBS(nes->cpu.p, 1, operand == 0);     // Z
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(operand, 7));  // N

            nes->cpu.a = operand;
            break;
        }
        case CMP: {
            // compare mem. val. (operand) to accumulator via subtraction
            // A - mem. val
            //set flags
            result = nes->cpu.a - operand;

            nes->cpu.p = ORBS(nes->cpu.p, 0, result >= 0);      // C
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result, 7));   // N
            break;
        }
        case SBC: {
            // accumulator is subtracted by a memory value and the NOT of carry flag
            // set carry if ~(result < $00)
            // set zero if result==0
            // set overflow if (result ^ A) & (result ^ ~memory) & $80
            // set negative to result bit 7
            result = nes->cpu.a - operand - ~BIT(nes->cpu.p, 0);

            nes->cpu.p = ORBS(nes->cpu.p, 0, ~result < 0x00);    // C
            nes->cpu.p = ORBS(nes->cpu.p, 1, result == 0);      // Z
            nes->cpu.p = ORBS(nes->cpu.p, 6, (result ^ nes->cpu.a) & (result ^ ~operand) & 0x80); // V
            nes->cpu.p = ORBS(nes->cpu.p, 7, BIT(result, 7));   // N

            nes->cpu.a = result;
            break;
        }
        default: break;
    }

    // ensure that instructions run for appropriate # of cycles
}

/* executes group 0b10 instructions */
static void ex_group02(NES *nes, Instr02 instruction) {

}

/* executes group 0b00 instructions */
static void ex_group03(NES *nes, Instr03 instruction) {

}

/* executes conditional branches instructions */
static void ex_cond(NES *nes, InstrCond instruction) {

}

/* executes the others, which aren't super easy to group */
static void ex_unformatted(NES *nes, uint8_t instruction) {

}

/*
 * adds general format for parsing 6502 instructions, including
 * setting up skeleton code for each group's instruction execution.
 *
 * format for instructions in groups 1,2, 3: aaabbbcc, where aaa & cc are the
 * op code, and bbb is the addressing mode.
 *
 * format for conditional instructions: xxy10000, where xx encodes the status flag
 * and y encodes the bit stored there.
 *
 * other instructions will be handled case by case, because they don't follow
 * a reliable format.
 *
 * more info here: https://llx.com/Neil/a2/opcodes.html
 */

void parse_instruction(NES *nes, uint8_t opcode){
    uint8_t group = EXT(opcode, 0, 2);

    if (group == 1) {
        Instr01 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group01(nes, instruction);

    } else if (group == 2) {
        Instr02 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group02(nes, instruction);

    } else if (group == 0) {
        Instr03 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group03(nes, instruction);

    } else if (EXT(opcode,0,5) == 16) {
        InstrCond instruction;
        instruction.flag = EXT(opcode, 6, 2);
        instruction.bit = BIT(opcode, 5);
        ex_cond(nes, instruction);
    } else {
        ex_unformatted(nes, opcode);
    }
}