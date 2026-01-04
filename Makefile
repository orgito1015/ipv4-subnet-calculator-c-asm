CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS :=

NAME := ipv4_calc

SRC_C := \
	src/main.c \
	src/ipv4_utils.c \
	src/subnetting.c

SRC_S := src/subnet_asm.S

OBJ := $(SRC_C:src/%.c=build/%.o) build/subnet_asm.o

all: $(NAME)

build:
	@mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

build/subnet_asm.o: $(SRC_S) | build
	$(CC) $(CFLAGS) -Iinclude -c $(SRC_S) -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	rm -rf build $(NAME)

.PHONY: all clean
