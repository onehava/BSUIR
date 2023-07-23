RELEASE = sort

LIBRARY1 = parse
LIBPATH1 = ../lib/parse

LIBRARY2 = index
LIBPATH2 = ../lib/index

SRC_DIR = .
AUX_DIR = ../aux
LIB_DIR = ../aux
OUT_DIR = ../bin

INCLUDE = ../include

EXE = $(OUT_DIR)/$(RELEASE)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(AUX_DIR)/$(RELEASE).%.o)
INC = $(foreach inc,$(INCLUDE),-I$(inc))

LIB1 = $(LIB_DIR)/lib$(LIBRARY1).a
LIB2 = $(LIB_DIR)/lib$(LIBRARY2).a

CC			= gcc
CPPFLAGS	= $(INC)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS		= -L$(LIB_DIR) -l$(LIBRARY1) -l$(LIBRARY2)

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) | $(LIB1) $(LIB2) $(OUT_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(LIB1):
	make -C $(LIBPATH1)

$(LIB2):
	make -C $(LIBPATH2)

$(AUX_DIR)/$(RELEASE).%.o: $(SRC_DIR)/%.c | $(AUX_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OUT_DIR) $(AUX_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ) $(EXE)
