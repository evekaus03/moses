#include <moses/moses.h>
#include "test_util.h"

static void check_all_addrs(uint16_t base, uint8_t expected) {
    assert(read_addr(nullptr, base) == expected);
    assert(read_addr(nullptr, base + 0x800) == expected);
    assert(read_addr(nullptr, base + 0x1000) == expected);
    assert(read_addr(nullptr, base + 0x1800) == expected);
}

/* The 2kB of internal RAM mapped from $0000 to $07FF
 * must be mirrored three times from $0800 to $1FFF. */
static void test_mirrored_ram() {
    write_addr(nullptr, 0x0000, 0xAB);
    check_all_addrs(0x0000, 0xAB);

    write_addr(nullptr, 0x0050, 0xFF);
    check_all_addrs(0x0050, 0xFF);

    write_addr(nullptr, 0x0550, 0xCA);
    check_all_addrs(0x0550, 0xCA);

    write_addr(nullptr, 0x1730, 0x30);
    check_all_addrs(0x0730, 0x30);

    write_addr(nullptr, 0x1910, 0x21);
    check_all_addrs(0x0110, 0x21);
}


int main() {
    test_mirrored_ram();
    return 0;
}