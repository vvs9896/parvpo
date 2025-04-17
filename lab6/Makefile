CC = gcc
CFLAGS = -Wall -Wextra -g

all: haiku_verifier

haiku_verifier.c: haiku_verifier.rl
	ragel -C -o haiku_verifier.c haiku_verifier.rl

haiku_verifier: haiku_verifier.c main.c
	$(CC) $(CFLAGS) haiku_verifier.c main.c -o haiku_verifier -Wno-unused-const-variable -Wsign-compare

test: haiku_verifier
	./haiku_verifier

clean:
	rm -f haiku_verifier.c haiku_verifier
