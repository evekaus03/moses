#include <moses/moses.h>
#include <stdlib.h>

#include "test_util.h"


static void test_bitwise() {
    uint8_t data = 0xff;
    assert(ORBS(data, 0, 0b0) == 0xfe);
    assert(ORBS(data, 1, 0b0) == 0xfd);
    assert(ORBS(data, 1, 0b1) == 0xff);
    assert(ORBS(ORBS(data, 7, 0), 7, 1) == 0xff);

    data = 0x0;
}

int main(int argc, char *argv[]) {
    test_bitwise();
    return 0;
}
