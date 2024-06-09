clean:
	rm -f d[0-9][0-9]
	rm -f *.o

d%: d%.o
	gcc -Wall -o $@ $?

d%.o: d%.c
	gcc -c -g -Wall -o $@ $?

d04: d04.rs
	rustc $<

d05: d05.go
	go build -gcflags=all="-N -l" -ldflags=-w $<
