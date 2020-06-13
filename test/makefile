CC=g++
IDIR=../
CFLAGS=-I$(IDIR)
OUT=out_test.out

doit:
	$(CC) -std=c++11 -Wall tests.cc -o $(OUT) $(CFLAGS)

op:
	$(CC) -std=c++11 -Wall op_test.cc -o $(OUT) $(CFLAGS)

run:
	./$(OUT) -r compact  -d yes

debug:
	$(CC) -ggdb -std=c++11 -Wall op_test.cc -o $(OUT) $(CFLAGS)