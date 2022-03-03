#ifndef __SPIKE_INTERFCES_H__
#define __SPIKE_INTERFCES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t rvv_new_processor(void);
int rvv_execute(uint64_t processor, uint64_t instruction);
void rvv_delete_processor(uint64_t);

#ifdef __cplusplus
}
#endif

#endif // __SPIKE_INTERFCES_H__

