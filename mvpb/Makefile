CC = gcc
CFLAGS = -Wall -Wextra -g3

all: src/output/Programa

src/output/Programa: src/Programa.c
	mkdir -p src/output
	$(CC) $(CFLAGS) src/Programa.c -o src/output/Programa

clean:
	rm -rf src/output

test: src/output/Programa
	cd src && ./output/Programa exemplo.pf

.PHONY: all clean test