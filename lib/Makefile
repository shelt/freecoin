ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

I_DIR = include
L_DIR = lib
O_DIR = build
S_DIR = src

CC=gcc
AR=ar
CFLAGS=-Wall -std=c99 -lpthread -I$(I_DIR) -Wno-unused-variable

# See "Secondary Expansion"
.SECONDEXPANSION:

default: init freecoin

# Generate object file
$(O_DIR)/%.o: $(S_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Main target
freecoin: $(addprefix $(O_DIR)/, $(addsuffix .o, $(basename $(shell cd $(S_DIR) && find $@ -name '*.c'))))
	$(AR) rcs $(L_DIR)/libfreecoin.a $^

.PHONY: clean
init:
	-mkdir -p $(O_DIR) $(L_DIR)
	-cd $(S_DIR) && find -type d -exec mkdir -p "../$(O_DIR)/{}" \;
clean:
	-rm -rf ./$(O_DIR)/*
	-rm -rf ./$(L_DIR)/*
