#pragma once
/*
* Defines functions which will be used to perform 6502 instructions
*/

#include <stdint.h>

#include "cpu.h"
#include "nes.h"

/*
 * this struct holds opcodes & addressing modes for group 1 instructions,
 * and can set the current op/mode using enum members.
 */
typedef struct {
    enum {
        ORA,
        AND,
        EOR,
        ADC,
        STA,
        LDA,
        CMP,
        SBC
    }op;

    enum {
        G1_INDIRECT_X,
        G1_ZERO_PAGE,
        G1_IMMEDIATE,
        G1_ABSOLUTE,
        G1_INDIRECT_Y,
        G1_ZERO_PAGE_X,
        G1_ABSOLUTE_Y,
        G1_ABSOLUTE_X
    }mode;
} Instr01;

/* similarly, a struct for group 2 instructions */
typedef struct {
    enum {
        ASL,
        ROL,
        LSR,
        ROR,
        STX,
        LDX,
        DEC,
        INC
    }op;

    enum {
        G2_IMMEDIATE,
        G2_ZERO_PAGE,
        G2_ACCUMULATOR,
        G2_ABSOLUTE,
        G2_ZERO_PAGE_X = 5,
        G2_ABSOLUTE_X = 7
    }mode;
} Instr02;

/* struct for group 3 instructions */
typedef struct {
    enum {
        BIT = 1,
        JMP,
        JMP_ABS,
        STY,
        LDY,
        CPY,
        CPX
    }op;

    enum {
        G3_IMMEDIATE,
        G3_ZERO_PAGE,
        G3_ABSOLUTE = 3,
        G3_ZERO_PAGE_X = 5,
        G3_ABSOLUTE_X = 7
    }mode;
} Instr03;

/* struct for conditional instructions, which use status register data */
typedef struct {
    enum {
        NEGATIVE,
        OVERFLOW,
        CARRY,
        ZERO
    }flag;

    uint8_t bit;
} InstrCond;

/* takes in 8-bit data to parse for which instruction it is. */
void parse_instruction(NES *nes, uint8_t opcode);

