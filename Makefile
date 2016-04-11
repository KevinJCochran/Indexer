all: test

test: test.c libindexer.a
	gcc -o test test.c libindexer.a

libindexer.a: sorted-index.o index-tokenizer.o
	ar r libindexer.a sorted-index.o index-tokenizer.o

sorted-index.o: sorted-index.c sorted-index.h
	gcc -c sorted-index.c

index-tokenizer.o: index-tokenizer.c index-tokenizer.h
	gcc -c index-tokenizer.c

clean:
	rm *.o 
	rm ./test
