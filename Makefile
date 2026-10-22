# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# Define targets.
TARGETS=search_index

# Source files
SRCS=main.c file.c heap.c trie.c operations.c
OBJS=$(SRCS:.c=.o)

# Manually define all targets.
build: $(TARGETS)

search_index: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o search_index

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run the program
run: build
	./search_index

# Pack the solution into a zip file.
pack:
	zip -FSr Serban-Nicolae-314CA.zip README* Makefile *.c *.h

# Clean the solution.
clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: build run pack clean
