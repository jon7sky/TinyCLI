INC = src/tcli
C_FILES = src/main.c src/tcli/tcli_def.c src/tcli/tcli_parse.c src/tcli/tcli_cmd_handle.c src/tcli/tcli_error.c
H_FILES = src/tcli/tcli.h src/tcli/tcli_def.h
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