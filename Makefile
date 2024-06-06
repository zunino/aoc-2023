clean:
	rm -f d[0-9][0-9]
	rm -f *.o

d%: d%.o
	gcc -Wall -o $@ $?

d%.o: d%.c
	gcc -c -g -Wall -o $@ $?
