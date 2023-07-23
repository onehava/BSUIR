SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = src

EXE1 = $(BIN_DIR)/child
EXE2 = $(BIN_DIR)/parent

SRC1 = src/child.c
SRC2 = src/parent.c

OBJ1 = $(SRC1:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ2 = $(SRC2:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC			= gcc
CPPFLAGS	= -I$(INC_DIR)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS		=
LDLIBS 		=

.PHONY: build child parent clean

build: child parent

child: $(EXE1)

parent: $(EXE2)

$(EXE1): $(OBJ1) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EXE2): $(OBJ2) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rf $(BIN_DIR) $(OBJ_DIR)