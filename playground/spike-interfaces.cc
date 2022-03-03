#include <string>
#include <iostream>
#include "spike-interfaces.h"
#include "processor.h"


uint64_t rvv_new_processor(void) {
    processor_t* proc = new processor_t("RV64GCV", "MSU", "vlen:128,elen:64", NULL, 0, false, NULL, std::cerr);
    return (uint64_t)proc;
}

int rvv_execute(uint64_t processor, uint64_t instruction) {
    processor_t* proc = (processor_t*)processor;
    try {
        insn_func_t func = proc->decode_insn(instruction);
        func(proc, instruction, 0);
    } catch (trap_t& e) {
        printf("Exception found: %s(V register %lu)\n", e.name(), e.cause());
    }
    return 0;
}

void rvv_delete_processor(uint64_t h) {
    delete (processor_t*)h;
}
