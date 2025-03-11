# Copyright @lucabotez

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=mk

#define object-files
OBJ=mk.o trie.o

build: $(TARGETS)

mk: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: clean
