SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = src

EXE = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC			= gcc
CPPFLAGS	= -I$(INC_DIR)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS		=
LDLIBS 		=

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rf $(BIN_DIR) $(OBJ_DIR)