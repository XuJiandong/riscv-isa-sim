#include <stdint.h>
#include <stdio.h>

int main() {
  printf("entering main() ...\n");

  uint64_t v1_values[4] = {0};
  uint64_t v2_values[4] = {1, 1, 1, 1};
  uint64_t v3_values[4] = {2, 2, 2, 2};

  uint64_t res_values[4] = {0};
  uint64_t avl = 4;
  uint64_t vl = 0;
  asm volatile("vsetvli t0, %1, e64, m2\n"
               "mv %0, t0"
               : "=r"(vl)
               : "r"(avl)
               : /* no registers */
  );

  asm volatile("vle64.v v2, (%0)" : : "r"(v1_values) :);
  asm volatile("vle64.v v10, (%0)" : : "r"(v2_values) :);
  asm volatile("vle64.v v20, (%0)" : : "r"(v3_values) :);
  asm volatile("vadd.vv v2, v10, v20");
  asm volatile("vse64.v v2, (%0)" : : "r"(res_values) :);

  printf("avl = %d, vl = %d\n", avl, vl);
  printf("result0 = %lld, result1 = %lld\n", res_values[0], res_values[1]);
  printf("leaving main()\n");
  return 0;
}
