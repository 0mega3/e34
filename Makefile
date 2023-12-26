#############################################################################
#
# Makefile to build e34.
#
# copyright 2023 by Denis Khorkin
# license   BSD-3-Clause
# author    Denis Khorkin <dkh01101111@gmail.com>
#############################################################################

CC = cc

WARNINGS	= -Wall
DEBUG		= -g
INCDIR		= -Iinclude
BSD 		= $(shell pkg-config --cflags libbsd-overlay)
CFLAGS 		= $(WARNINGS) $(DEBUG) $(INCDIR) $(BSD)

NCURSESLIB	= -lncurses
LIBC 		= -lc
LIBBSD 		= $(shell pkg-config --libs libbsd-overlay)
LDFLAGS		= $(NCURSESLIB) $(LIBC) $(LIBBSD)

SRC = $(shell find . -type f -name '*.c' | grep -v .ccls-cache)
OBJ = $(SRC:.c=.o)
OUT = e34

all: $(OUT)

## build exec ##
$(OUT): $(OBJ)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)

## remove things ##
.PHONY: clean
clean:
	rm -f $(OBJ) $(OUT)
