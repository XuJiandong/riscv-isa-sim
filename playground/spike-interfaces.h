#ifndef __SPIKE_INTERFCES_H__
#define __SPIKE_INTERFCES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t rvv_new_processor(void);
int rvv_execute(uint64_t processor, uint64_t instruction);
/**
 *  offset: the offset in register file in bytes
 *
 *  return: 0, success; otherwise, failed. Same to other functions
 */
int rvv_get_vreg(uint64_t processor, uint64_t offset, uint8_t *mem, uint64_t mem_size);
int rvv_set_vreg(uint64_t processor, uint64_t offset, uint8_t *mem, uint64_t mem_size);

int rvv_get_xreg(uint64_t processor, uint64_t index, uint64_t *content);
int rvv_set_xreg(uint64_t processor, uint64_t index, uint64_t content);

/**
 * addr: the address of memory. The memory is in environment of risc-v execution.
 * len: the length of memory
 * bytes: the buffer to hold the content loaded from memory. It should have same length with `len`
 * note:
 * don't try to access the memory at [0, 4096]
  // Disallow access to debug region when not in debug mode
  if (addr >= DEBUG_START && addr <= DEBUG_END && proc && !proc->state.debug_mode)
    return false;
 */
int rvv_load_mem(uint64_t processor, uint64_t addr, uint64_t len, uint8_t *bytes);
int rvv_store_mem(uint64_t processor, uint64_t addr, uint64_t len, uint8_t *bytes);

uint64_t rvv_get_vlen(uint64_t processor);
uint64_t rvv_get_elen(uint64_t processor);
uint64_t rvv_get_vl(uint64_t processor);
uint64_t rvv_get_sew(uint64_t processor);
uint64_t rvv_get_vtype(uint64_t processor);
uint64_t rvv_get_lmul(uint64_t processor);
uint64_t rvv_get_vill(uint64_t processor);

void rvv_delete_processor(uint64_t);

#ifdef __cplusplus
}
#endif

#endif // __SPIKE_INTERFCES_H__
