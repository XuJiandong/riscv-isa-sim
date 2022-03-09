

sed -i '/softfloat_install_shared_lib/d' ../softfloat/softfloat.mk.in
sed -i 's/private:/public:/g' ../riscv/processor.h
sed -i 's/private:/public:/g' ../riscv/mmu.h
