all: test

test: test.c libindexer.a
	gcc -o test test.c libindexer.a

libindexer.a: sorted-index.o
	ar r libindexer.a sorted-index.o

sorted-index.o: sorted-index.c sorted-index.h
	gcc -c sorted-index.c

clean:
	rm *.o 
	rm ./test
