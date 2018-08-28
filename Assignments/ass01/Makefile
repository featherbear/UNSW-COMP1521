# COMP1521 18s2 Assignment 1

CC	= cc
CFLAGS	= -g -Wall -Werror -std=c99

.PHONY: all
all: C_Code/scroll scroll.s

scroll: C_Code/scroll.o
C_Code/scroll.o: C_Code/scroll.c C_Code/chars.h

scroll.s: #components_asm/pre/*.s components_asm/*.s
	@cat components_asm/pre/header.text > $@
	@cat `find components_asm/pre -maxdepth 1 -type f -iname "*.s" | sort -V` >> $@
	@cat components_asm/*.s >> $@
.PHONY: clean
clean:
	rm -f C_Code/scroll C_Code/*.o scroll.s # core
