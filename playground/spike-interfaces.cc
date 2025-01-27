#include "spike-interfaces.h"
#include "mmu.h"
#include "processor.h"
#include "simif.h"
#include <iostream>
#include <string>
// memory

// this is the interface to the simulator used by the processors and memory
class memory : public simif_t {
  public:
    memory(uint64_t size) {
        mem = new uint8_t[size];
        mem_size = size;
    }
    ~memory() { delete[] mem; }
    virtual char *addr_to_mem(reg_t addr) { return NULL; }
    virtual bool mmio_load(reg_t addr, size_t len, uint8_t *bytes) {
        if ((addr + len) > mem_size) {
            return false;
        }
        memcpy(bytes, mem + addr, len);
        return true;
    }
    virtual bool mmio_store(reg_t addr, size_t len, const uint8_t *bytes) {
        if ((addr + len) > mem_size) {
            return false;
        }
        memcpy(mem + addr, bytes, len);

        return true;
    }
    virtual void proc_reset(unsigned id) {}
    virtual const char *get_symbol(uint64_t addr) { return NULL; }

  private:
    uint8_t *mem;
    uint64_t mem_size;
};

uint64_t rvv_new_processor(void) {
    memory *mem = new memory(4 * 1024 * 1024);
    processor_t *proc = new processor_t("RV64GCV", "MSU", "vlen:128,elen:64", mem, 0, false, NULL, std::cerr);
    reg_t val = proc->state.sstatus->read();
    proc->state.sstatus->write(val | SSTATUS_VS);
    return (uint64_t)proc;
}

int rvv_execute(uint64_t processor, uint64_t instruction) {
    processor_t *proc = (processor_t *)processor;
    try {
        insn_func_t func = proc->decode_insn(instruction);
        func(proc, instruction, 0);
    } catch (trap_t &e) {
        fprintf(stderr, "Exception found, error code: %lu(%s), instruction: 0x%08lX)\n", e.cause(), e.name(), e.get_tval());
        // `cause` is starting from zero, see `CAUSE_MISALIGNED_FETCH`
        return (int)e.cause() + 1;
    }
    return 0;
}

int rvv_get_vreg(uint64_t processor, uint64_t offset, uint8_t *mem, uint64_t mem_size) {
    processor_t *proc = (processor_t *)processor;
    uint8_t *reg_file = (uint8_t *)proc->VU.reg_file;
    uint64_t total_size = NVPR * proc->VU.vlenb;
    if ((offset + mem_size) > total_size) {
        fprintf(stderr, "out of bounds: offset = %lu, mem_size = %lu, total_size = %lu\n", offset, mem_size, total_size);
        return -1;
    }
    memcpy(mem, reg_file + offset, mem_size);

    return 0;
}

int rvv_set_vreg(uint64_t processor, uint64_t offset, uint8_t *mem, uint64_t mem_size) {
    processor_t *proc = (processor_t *)processor;
    uint8_t *reg_file = (uint8_t *)proc->VU.reg_file;
    uint64_t total_size = NVPR * proc->VU.vlenb;
    if ((offset + mem_size) > total_size) {
        fprintf(stderr, "out of bounds: offset = %lu, mem_size = %lu, total_size = %lu\n", offset, mem_size, total_size);
        return -1;
    }
    memcpy(reg_file + offset, mem, mem_size);

    return 0;
}

int rvv_get_xreg(uint64_t processor, uint64_t index, uint64_t *content) {
    processor_t *proc = (processor_t *)processor;
    if (index >= NXPR) {
        fprintf(stderr, "error, out of bounds: %lu >= NXPR\n", index);
        return -1;
    }
    *content = proc->state.XPR[index];
    return 0;
}

int rvv_set_xreg(uint64_t processor, uint64_t index, uint64_t content) {
    processor_t *proc = (processor_t *)processor;
    if (index >= NXPR) {
        fprintf(stderr, "error, out of bounds: %lu >= NXPR\n", index);
        return -1;
    }
    proc->state.XPR.write(index, content);
    return 0;
}

uint64_t rvv_get_vlen(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.VLEN;
}

uint64_t rvv_get_elen(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.ELEN;
}

uint64_t rvv_get_vl(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.vl->read();
}

uint64_t rvv_get_sew(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.vsew;
}

uint64_t rvv_get_vtype(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.vtype->read();
}

uint64_t rvv_get_lmul(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.vflmul;
}

uint64_t rvv_get_vill(uint64_t processor) {
    processor_t *proc = (processor_t *)processor;
    return proc->VU.vill;
}

void rvv_delete_processor(uint64_t h) { delete (processor_t *)h; }

int rvv_load_mem(uint64_t processor, uint64_t addr, uint64_t len, uint8_t *bytes) {
    processor_t *proc = (processor_t *)processor;
    mmu_t *mmu = proc->get_mmu();
    if (addr < 4096) {
        return -4;
    }
    bool success = mmu->mmio_load(addr, len, bytes);
    if (success) {
        return 0;
    } else {
        return -2;
    }
}

int rvv_store_mem(uint64_t processor, uint64_t addr, uint64_t len, uint8_t *bytes) {
    processor_t *proc = (processor_t *)processor;
    if (addr < 4096) {
        return -4;
    }
    mmu_t *mmu = proc->get_mmu();
    bool success = mmu->mmio_store(addr, len, bytes);
    if (success) {
        return 0;
    } else {
        return -3;
    }
}
