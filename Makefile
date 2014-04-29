all: svd

svd: svd_eigen.cpp
	g++ -O3 -o $@ $^ -I.

clean: 
	rm -f svd
