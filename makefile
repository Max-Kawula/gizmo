#maxmakefile 1.1
#made for linux
#expects for raylib.h and librarylib.so to be installed in /usr/local/include /usr/local/lib

#compiler settings
CC		?= gcc
CFLAGS		?= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra -Werror -pedantic
BUILD_MODE	?= RELEASE# RELEASE or DEBUG

#file directories
SRCDIR		?= src
OBJ_D		?= obj#temporarily named that way
BIN_D		?= bin
DEBUGDIR	?= debug

#build mode settings, OBJD and BIND are temporary variables
ifeq ($(BUILD_MODE),RELEASE)
	CFLAGS	+= -O2
	OBJDIR	 = $(OBJ_D)
	BINDIR	 = $(BIN_D)
else
	CFLAGS	+= -g -O0
	OBJDIR	 = $(DEBUGDIR)/$(OBJ_D)
	BINDIR	 = $(DEBUGDIR)/$(BIN_D)
endif

SRC_FILES	 = $(wildcard $(SRCDIR)/*.c)
OBJ_FILES	 = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC_FILES))
BIN		?= output

.phony: clean all

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $^ $(CFLAGS) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $^ $(CFLAGS) -c -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/* $(DEBUGDIR)/$(BINDIR)/* $(DEBUGDIR)/$(OBJDIR)/*
run: 
	$(BINDIR)/$(BIN)

rungdb:
	gdb $(DEBUGDIR)/$(BINDIR)/$(BIN)
