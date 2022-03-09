
CC = /opt/riscv_rvv/bin/riscv64-unknown-elf-gcc
OBJDUMP = /opt/riscv_rvv/bin/riscv64-unknown-elf-objdump


SPIKE ?= ../build/spike
PK ?= /opt/riscv/riscv64-unknown-elf/bin/pk
V_ISA ?= RV64GCV
RVV_AS ?= rvv-as

all: hello

hello: hello.c
	$(CC) -o $@ $<

run-hello: hello
	$(SPIKE) --isa=$(V_ISA) $(PK) hello

debug-hello: hello
	lldb $(SPIKE) -- --isa=RV64GCV $(PK) hello

as:
	$(RVV_AS) vadd_vv.s

asm-mem.o: asm-mem.s
	$(CC) -c -o $@ $<

dump: asm-mem.o
	$(OBJDUMP) -S $<
