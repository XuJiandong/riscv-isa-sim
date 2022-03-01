
CC = /opt/riscv_rvv/bin/riscv64-unknown-elf-gcc
SPIKE ?= ../build/spike
PK ?= /opt/riscv/riscv64-unknown-elf/bin/pk
V_ISA ?= RV64GCV


all: hello

hello: hello.c
	$(CC) -o $@ $<

run-hello: hello
	$(SPIKE) --isa=$(V_ISA) $(PK) hello
