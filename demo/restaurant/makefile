TCLI_DIR = ../../tcli
INC = $(TCLI_DIR)
C_FILES = main.c $(TCLI_DIR)/tcli_def.c $(TCLI_DIR)/tcli_parse.c $(TCLI_DIR)/tcli_cmd_handle.c $(TCLI_DIR)/tcli_error.c
H_FILES = $(TCLI_DIR)/tcli.h $(TCLI_DIR)/tcli_def.h
EXE_FILE = tcli.exe
CC = gcc
RM = del
CFLAGS = -Wall

$(EXE_FILE): $(C_FILES) $(H_FILES) Makefile
	$(CC) -o $(EXE_FILE) -I$(INC) $(CFLAGS) $(C_FILES)

.PHONY: all
all: $(EXE_FILE)

.PHONY: clean
clean:
	$(RM) $(EXE_FILE)