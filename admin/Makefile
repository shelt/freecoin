I_DIR = include
L_DIR = lib
O_DIR = build
B_DIR = bin
S_DIR = src

BIN_NAME=freecoin-admin

CC=gcc
CFLAGS=-Wall -std=c99 -lpthread -lfreecoin -I$(I_DIR) -Wno-unused-variable

default: init main

# See "Secondary Expansion"
.SECONDEXPANSION:

# Generate object file
$(O_DIR)/%.o: $(S_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

main: $(addprefix $(O_DIR)/, $(addsuffix .o, $(basename $(shell cd $(S_DIR) && find $@ -name '*.c'))))
	$(CC) $^ -o $(B_DIR)/$(BIN_NAME) $(CFLAGS)

.PHONY: clean
init:
	-mkdir -p $(O_DIR) $(B_DIR)
	-cd $(S_DIR) && find -type d -exec mkdir -p "../$(O_DIR)/{}" \;
clean:
	-rm -rf ./$(O_DIR)/*
	-rm -rf ./$(B_DIR)/*
	-rm -rf ./$(L_DIR)/*
