# before building it, run build-lib.sh

CXX := c++
CXXFLAGS := -g -O0  -fPIC -I../riscv  -I../build -I../softfloat -I../fesvr
LDFLAGS := -L'../build' -lriscv -lsoftfloat -ldisasm -ldl

all: example mem

example: example.o spike-interfaces.o
	$(CXX) $^ -o $@ $(LDFLAGS)

mem: mem.o spike-interfaces.o
	$(CXX) $^ -o $@ $(LDFLAGS)

example.o: example.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

mem.o: mem.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

spike-interfaces.o: spike-interfaces.cc spike-interfaces.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

run-example: example
	LD_LIBRARY_PATH=../build ./example

run-mem: mem
	LD_LIBRARY_PATH=../build ./mem

fmt:
	clang-format -i -style="{IndentWidth: 4, ColumnLimit: 140}" *.cc *.h

clean:
	rm -f *.o
	rm -f example
	rm -f mem
