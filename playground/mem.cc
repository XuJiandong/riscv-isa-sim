#include "spike-interfaces.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    int err = 0;
    uint64_t addr = 0x10000;
    uint64_t val = 17;
    uint64_t proc = rvv_new_processor();
    //    2:   62c1                    lui     t0,0x10
    //    4:   4345                    li      t1,17
    //    6:   0062b023                sd      t1,0(t0) # 10000 <mem+0xfffe>
    //    a:   0002b303                ld      t1,0(t0)

    // t0 hold address of "0x10000"(addr)
    // t1 hold the value, initially 17(val)
    err = rvv_execute(proc, 0x62c1);
    assert(err == 0);
    err = rvv_execute(proc, 0x4345);
    assert(err == 0);
    // store t1 to addr
    err = rvv_execute(proc, 0x0062b023);
    assert(err == 0);

    // check the content of addr
    uint64_t val_mem = 0;
    err = rvv_load_mem(proc, addr, 8, (uint8_t *)&val_mem);
    assert(err == 0);
    assert(val_mem == val);

    // modify the content of addr
    uint64_t new_val = 18;
    err = rvv_store_mem(proc, addr, 8, (uint8_t *)&new_val);
    assert(err == 0);

    // load the content of addr to t1
    err = rvv_execute(proc, 0x0002b303);
    assert(err == 0);

    // check the conent of t1
    uint64_t t1 = 0;
    err = rvv_get_xreg(proc, 6, &t1);
    assert(err == 0);
    assert(t1 == 18);

    printf("done\n");
    return 0;
}
