d01: d01.o
	gcc -Wall -o $@ $?

d01.o: d01.c
	gcc -c -Wall -o $@ $?
