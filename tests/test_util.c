#include "test_util.h"
#include <stdlib.h>

void assert(bool cond) {
    if (!cond) {
        exit(1);
    }
}
