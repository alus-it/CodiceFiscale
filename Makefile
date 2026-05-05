# ============================================================================
# Name        : Makefile
# Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
# Copyright   : (C) 2004 Alberto Realis-Luc
# License     : GNU GPL v2
# Repository  : https://github.com/alus-it/CodiceFiscale
# Web         : http://www.alus.it/pubs/CodiceFiscale
# Description : Makefile of CF program to generate Italian fiscal codes
# ============================================================================

# CF version string
VERSION = 1.5

# Compiler and linker options
WARN_CFLAGS = -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wno-unused-parameter -Werror
CFLAGS = -c -O3 -fPIC $(WARN_CFLAGS) #-std=gnu99
#LFLAGS =

# Source and binary paths
SRC = src/
BIN = bin/

# Headers for others external libs path (TomTom stuff)
#INC = include/

# List of C source files
CFILES =   \
	cf.c   \
	main.c

# List of object files
OBJS = $(patsubst %.c, $(BIN)%.o, $(CFILES))

# Final executable destination folder
DIST = ./

### Build
all: $(DIST)cf

$(DIST)cf: $(OBJS)
	@echo Linking all into: $@
	@gcc $(LFLAGS) $(OBJS) -o $@
	@strip $@

# Create bin directory if missing
$(OBJS): | $(BIN)
$(BIN):
	mkdir -p $(BIN)

$(BIN)main.o: $(SRC)main.c $(SRC)cf.h
	@echo Compiling: $<
	@gcc $(CFLAGS) -D'VERSION="$(VERSION)"' $< -o $@

$(BIN)cf.o: $(SRC)cf.c $(SRC)cf.h
	@echo Compiling: $<
	@$gcc $(CFLAGS) $< -o $@

### Clean
clean:
	@echo Cleaning: objects and executable
	@rm -rf $(BIN)
	@rm -f $(DIST)cf
