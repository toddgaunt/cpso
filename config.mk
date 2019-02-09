VERSION := 1.0.0

# System paths
PREFIX := /usr/local
BINPREFIX := $(PREFIX)/bin
INCLUDEPREFIX := $(PREFIX)/include
LIBPREFIX := $(PREFIX)/lib
MANPREFIX := $(PREFIX)/man

# Linking flags
LDFLAGS :=

# C Compiler settings
CC := cc
CFLAGS := -O2 -Iinclude -std=c89 -ansi -Wall -Wextra
#CFLAGS += -O0 -g
