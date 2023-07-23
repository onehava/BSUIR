RELEASE = cmd

SRC_DIR = .
OUT_DIR = ../../aux
AUX_DIR = ../../aux

INCLUDE = ../../include

LIB = $(OUT_DIR)/lib$(RELEASE).a
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(AUX_DIR)/$(RELEASE).%.o)
INC = $(foreach inc,$(INCLUDE),-I$(inc))

CC			= gcc
CPPFLAGS 	= $(INC)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

.PHONY: build clean

build: $(LIB)

$(LIB): $(OBJ) | $(OUT_DIR)
	ar rc $@ $^
	ranlib $@

$(AUX_DIR)/$(RELEASE).%.o: $(SRC_DIR)/%.c | $(AUX_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OUT_DIR) $(AUX_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ) $(LIB)
