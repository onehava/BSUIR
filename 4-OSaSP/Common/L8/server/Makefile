RELEASE = server

LIBRARY = cmd
LIBPATH = ../lib/cmd

SRC_DIR = .
AUX_DIR = ../aux
LIB_DIR = ../aux
OUT_DIR = ../bin

INCLUDE = ../include

EXE = $(OUT_DIR)/$(RELEASE)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(AUX_DIR)/$(RELEASE).%.o)
INC = $(foreach inc,$(INCLUDE),-I$(inc))

LIB = $(LIB_BIR)/lib$(LIBRARY).a

CC			= gcc
CPPFLAGS	= $(INC)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS		= -L$(LIB_DIR) -l$(LIBRARY)

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) | $(LIB) $(OUT_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(LIB):
	make -C $(LIBPATH)

$(AUX_DIR)/$(RELEASE).%.o: $(SRC_DIR)/%.c | $(AUX_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OUT_DIR) $(AUX_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ) $(EXE)
