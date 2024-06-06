clean:
	rm -f d[0-9][0-9]
	rm -f *.o

%: %.o
	gcc -Wall -o $@ $?

%.o: %.c
	gcc -c -g -Wall -o $@ $?
