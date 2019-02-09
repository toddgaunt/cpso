# See LICENSE file for copyright and license details.

# Project configuration
include config.mk

TARGET := cpso
MODULES := 
SRC := cpso.c

# Project modules
include $(patsubst %, %/module.mk, $(MODULES))

OBJ := $(patsubst %.c, %.o, $(filter %.c, $(SRC)))
OBJ_D := $(patsubst %.c, %.o_d, $(filter %.c, $(SRC)))
TEST := $(patsubst %.c, %_test, $(filter %.c, $(SRC)))

# Standard targets
all: lib$(TARGET).a
debug: lib$(TARGET).a_d

options:
	@echo "Build options:"
	@echo "CFLAGS    = $(CFLAGS)"
	@echo "LDFLAGS   = $(LDFLAGS)"
	@echo "CFLAGS_D  = $(CFLAGS_D)"
	@echo "LDFLAGS_D = $(LDFLAGS_D)"
	@echo "CC        = $(CC)"

clean:
	@echo "Cleaning"
	@rm -f $(OBJ)
	@rm -f $(OBJ_D)
	@rm -f $(TEST)
	@rm -f $(TARGET)
	@rm -f $(TARGET)_d
	@rm -f lib$(TARGET).a
	@rm -f lib$(TARGET).a_d

test: $(TEST)

check: test
	@for t in $(TEST); do ./$$t; done

vcheck: test
	@for t in $(TEST); do valgrind -q ./$$t; done

# Object Build Rules
%.o: %.c config.mk
	@echo "CC [R] $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o_d: %.c config.mk
	@echo "CC [D] $@"
	@$(CC) $(CFLAGS_D) -c -o $@ $<

# Targets
%_test: %_test.o_d lib$(TARGET).a_d
	@echo "CC $@"
	@$(CC) -o $@ $^ $(LDFLAGS)

lib$(TARGET).a: $(OBJ)
	@echo "AR [R] $@ "
	@ar -cq $@ $?

lib$(TARGET).a_d: $(OBJ_D)
	@echo "AR [D] $@"
	@ar -cq $@ $?

.PHONY: all options clean
