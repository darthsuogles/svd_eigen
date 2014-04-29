
EIGEN_DIR=$(PWD)/eigen/3.2.1

all: svd_pinv mmio.o

mmio.c: mmio.h

%.o: %.c
	g++ -O3 -c $^

svd_pinv: svd_pinv.cpp mmio.o
	g++ -O3 -o $@ $^ -I$(EIGEN_DIR)

clean: 
	rm -f svd_pinv *.o
