#include <stdint.h>
#include <stdio.h>

int main() {
  printf("entering main() ...\n");

  uint64_t avl = 100;
  uint64_t vl = 0;
  // can't support when lmul = 2, 4, 8
  asm volatile("vsetvli t0, %1, e32, m1\n"
				"mv %0, t0"
               : "=r" (vl)
               : "r"(avl)
               : /* no registers */
  );
  printf("avl = %d, vl = %d\n", avl, vl);
  asm volatile("vadd.vv v21, v1, v11");
  int i = 100;
  printf("leaving main()\n");
  return 0;
}
