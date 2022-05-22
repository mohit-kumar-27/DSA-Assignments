a.out : SPheap_main.o SPheap_implementation.o
	gcc SPheap_main.o SPheap_implementation.o -lm

SPheap_main.o : SPheap_main.c SPheap_header.h
	gcc -c SPheap_main.c

SPheap_implementation.o : SPheap_implementation.c SPheap_header.h
	gcc -c SPheap_implementation.c -lm
