

CXX := c++
CXXFLAGS := -g -O0  -fPIC -I../riscv  -I../build -I../softfloat
LDFLAGS := -L'../build' -lriscv -lsoftfloat -ldisasm -ldl

all: example

example: example.o spike-interfaces.o
	$(CXX) $^ -o $@ $(LDFLAGS)

example.o: example.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

spike-interfaces.o: spike-interfaces.cc spike-interfaces.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

run-example: example
	LD_LIBRARY_PATH=../build ./example

clean:
	rm -f *.o
	rm -f example
