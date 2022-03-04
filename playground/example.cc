#include <assert.h>
#include <stdio.h>

#include "spike-interfaces.h"

int main(int argc, const char *argv[]) {
  uint64_t proc = rvv_new_processor();
  uint64_t vlen = rvv_get_vlen(proc);
  uint64_t elen = rvv_get_elen(proc);
  printf("vlen = %lu, elen = %lu\n", vlen, elen);
  uint64_t vlenb = vlen / 8;
  uint64_t value_one[2] = {1, 1};
  uint64_t result[2] = {0};

  uint64_t insn = 0xc18472d7; // vsetivli t0, 8, e64, m1
  int i = rvv_execute(proc, insn);

  uint64_t vl = rvv_get_vl(proc);
  uint64_t sew = rvv_get_sew(proc);
  uint64_t lmul = rvv_get_lmul(proc);
  uint64_t vill = rvv_get_vill(proc);
  printf("vl = %lu, sew = %lu, lmul = %lu, vill = %lu\n", vl, sew, lmul, vill);

  rvv_set_registers(proc, vlenb * 10, (uint8_t *)value_one, sizeof(value_one));
  rvv_set_registers(proc, vlenb * 20, (uint8_t *)value_one, sizeof(value_one));

  insn = 0x02aa0157; // vadd.vv v2, v10, v20
  i = rvv_execute(proc, insn);

  rvv_get_registers(proc, vlenb * 2, (uint8_t *)result, sizeof(result));
  printf("result[0] = %lu, result[1] = %lu\n", result[0], result[1]);
  assert(result[0] == 2);
  assert(result[1] == 2);

  rvv_delete_processor(proc);
  return 0;
}
