#include <moses/moses.h>
#include <stdint.h>

static uint8_t ram[0x800];
static uint8_t ppu[8];
static uint8_t io[0x18];
static uint8_t test_mode_io[0x8];

uint8_t read_addr(NES *nes, uint16_t addr) {
    // note: as components are properly emulated, this will
    // be changed to provide their proper read/write behavior
    if (addr < 0x2000) {
        return ram[addr & 0x7FF];
    }
    if (addr < 0x4000) {
        return ppu[addr & 8];
    }
    if (addr < 0x4018) {
        return io[addr - 0x4000];
    }
    if (addr < 0x4020) {
        return test_mode_io[addr - 0x4018];
    }
    // TODO: unmapped cartridge area

    return 0;
}

void write_addr(NES *nes, uint16_t addr, uint8_t data) {
    // note: as components are properly emulated, this will
    // be changed to provide their proper read/write behavior
    if (addr < 0x2000) {
        ram[addr & 0x7FF] = data;
    } else if (addr < 0x4000) {
        ppu[addr & 8] = data;
    } else if (addr < 0x4018) {
        io[addr - 0x4000] = data;
    } else if (addr < 0x4020) {
        test_mode_io[addr - 0x4020] = data;
    }
    // TODO: unmapped cartridge area
}