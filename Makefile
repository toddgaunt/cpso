# See LICENSE file for copyright and license details.

# Project configuration
include config.mk

MODULES :=
SRC := cpso.c

# Project modules
include $(patsubst %, %/module.mk, $(MODULES))

OBJ := $(patsubst %.c, %.o, $(filter %.c, $(SRC)))

# Standard targets
all: libcpso.a

options:
	@echo "Build options:"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

clean:
	@echo "Cleaning"
	@rm -rf $(OBJ)
	@rm -f example.o
	@rm -f example
	@rm -f example-vis
	@rm -f libcpso.a

install: libcpso.a cpso.h
	@echo "Installing..."
	@cp -f cpso.h $(INCLUDEPREFIX)
	@cp -f libcpso.a $(LIBPREFIX)

uninstall:
	@echo "Uninstalling..."
	@rm -f $(INCLUDEPREFIX)/cpso.h
	@rm -f $(LIBPREFIX)/libcpso.a

# Object Build Rules
%.o: %.c config.mk
	@echo "CC [R] $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

# Targets
libcpso.a: $(OBJ)
	@echo "Creating library archive for release... "
	@ar -cq $@ $?
	@echo "done."

example: example.o $(OBJ)
	@echo "CC $@"
	@$(CC) -o $@ $^ $(LDFLAGS) -lm
	
example-vis: example_vis.o $(OBJ)
	@echo "CC $@"
	@$(CC) -o $@ $^ $(LDFLAGS) -lm -lSDL2 -lSDL2_image

.PHONY: all options clean
