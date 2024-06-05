#maxmakefile 1.0
#special variables corrleate to the rule x:y $@:$^

CC=gcc
CFLAGS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra -Werror -pedantic
SRCDIR=src
OBJDIR=obj
BINDIR=bin
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
BIN=$(BINDIR)/main



all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $^ $(CFLAGS) -c -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
run: 
	$(BIN)
