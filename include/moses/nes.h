#pragma once

/*
This provides the interface for controlling the entire emulated NES system as a whole.
*/

#include <stdint.h>

#include "mapper.h"
#include "cpu.h"
#include "apu.h"
#include "ppu.h"

#define NES_WRAMSIZE	2048

// Reference: https://www.nesdev.org/wiki/Cycle_reference_chart
#define CLOCK_HZ_MASTER_NTSC	(236250000 / 11)
#define CLOCK_HZ_MASTER_PAL		26601712
#define CLOCK_CPU_HZ_NTSC		(MASTER_CLOCK_HZ_NTSC / 12)
#define CLOCK_CPU_HZ_PAL		(MASTER_CLOCK_HZ_NTSC / 16)

typedef struct NES {
	CPU cpu;
	uint8_t * wram;
	MapperBase * mapper;
	PPU ppu;
	APU apu;
	uint64_t master_ticks_elapsed;
} NES;

// Allocates an NES struct, performs a system reset on it, and returns a pointer to the struct.
// If provided a non-NULL pointer to a MapperBase-compatible structure,
// the system will be initialized with that mapper.
// Otherwise, the will be initialized to an 'open bus'/empty cartridge state.
NES * nes_create(MapperBase * mapper);

void nes_delete(NES * nes);

// Puts the NES into a state as if a full reset had been performed.
void nes_reset(NES * nes);

void nes_attach_mapper(NES * nes, MapperBase * mapper);

// Run the emulation up to and including the point of drawing the full frame and calling the client's frame presentation function.
void nes_emulate_next_frame();

void nes_emulate_master_ticks(uint64_t n_ticks);

// Note: ticking the CPU forward also ticks other components forward and can trigger events in those components off of the CPU tick.
void nes_emulate_cpu_ticks(uint64_t n_ticks);

// Execute the next CPU instruction, also ticking forward other
// components in the time elapsed.
void nes_step_next_instruction();

// Execute up to, but not including the given instruction address.
// If the instruction before this address would advance the PC past it
// (possible if the address is the 2nd byte of a 2-byte instruction),
// then the execution will stop just before *that* instruction instead.
// For example:
// 			[Inst 1]
// addr =>	[operand]
// 			[Inst 2]
// Becomes:
// addr =>	[Inst 1]
// 			[operand]
// 			[Inst 2]
void nes_run_to_instruction(NES *nes, reg16 addr);

/* Retrieve the byte at the specified address
 * in the memory map.
 */
uint8_t read_addr(NES *nes, uint16_t addr);

/* Write the specified byte to the specified
 * address in the memory map.
 */
void write_addr(NES *nes, uint16_t addr, uint8_t data);

