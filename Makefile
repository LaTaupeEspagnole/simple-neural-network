CC= gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -Iincludes
LDFLAGS= -L. -lpthread -lm -lmatrix

VPATH= src tests

OBJS= main.o buffer.o error_msg.o network.o
LIB= libmatrix.a

.PHONY: clean


# --- Main targets ---
all: main

main: CFLAGS += -O3
main: $(OBJS) $(LIB)

debug: CFLAGS += -g
debug: main

# --- Cleanng targets ---

clean-all: clean clean-deps

clean:
	$(RM) $(OBJS) main

clean-deps:
	$(RM) libmatrix.a includes/libmatrix.h
