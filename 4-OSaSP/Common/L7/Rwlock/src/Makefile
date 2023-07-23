RELEASE = main

SRC_DIR = .
AUX_DIR = ../aux
OUT_DIR = ../bin

INCLUDE = ../include

EXE = $(OUT_DIR)/$(RELEASE)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(AUX_DIR)/$(RELEASE).%.o)
INC = $(foreach inc,$(INCLUDE),-I$(inc))

CC			= gcc
CPPFLAGS	= $(INC)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS		=

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) | $(OUT_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(AUX_DIR)/$(RELEASE).%.o: $(SRC_DIR)/%.c | $(AUX_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OUT_DIR) $(AUX_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ) $(EXE)