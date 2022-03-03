
#include "spike-interfaces.h"

int main(int argc, const char* argv[]) {
    uint64_t proc = rvv_new_processor();

    uint64_t insn = 0xc18472d7; // vsetivli t0, 8, e64, m1
    int i = rvv_execute(proc, insn);

    insn = 0x02aa0157; // vadd.vv v2, v10, v20
    i = rvv_execute(proc, insn);

    rvv_delete_processor(proc);
    return 0;
}
