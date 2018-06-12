CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra -pedantic -I./headers

VPATH=./src

SRC=my_readelf.c print_header.c struct_opt.c
OBJ=$(SRC:.c=.o)

all: my_readelf

my_readelf: $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f my_readelf
