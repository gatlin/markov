all:
	gcc -o markov src/*.c -Isrc -I/usr/include/hiredis -L/usr/lib -lhiredis

