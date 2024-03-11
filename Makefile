CC = gcc
CC_FLAGS = -Wall -Werror -g
LDFLAGS = -lm

build: magic-band

tema1: magic-band.c magic-band.h
	$(CC) $(CC_FLAGS) magic-band.c -o magic-band $(LDFLAGS)

clean:
	rm -f magic-band.o

run: build
	./magic-band
