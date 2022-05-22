a.out: A2.o SPheap_implementation.o
	gcc A2.o SPheap_implementation.o -lm

A2.o: A2.c SPheap_header.h
	gcc -c A2.c -lm

SPheap_implementation.o: SPheap_implementation.c SPheap_header.h
	gcc -c SPheap_implementation.c -lm
