
# COMP1521 18s1 Lab02 Makefile

CC=gcc
CFLAGS=-Wall -Werror

add : add.o BigNum.o
	$(CC) -o add add.o BigNum.o

multiply : multiply.o BigNum.o
	$(CC) -o multiply multiply.o BigNum.o

divide : divide.o BigNum.o
	$(CC) -o divide divide.o BigNum.o

subtract : subtract.o BigNum.o
	$(CC) -o subtract subtract.o BigNum.o

add.o : add.c BigNum.h
multiply.o : multiply.c BigNum.h
divide.o : divide.c BigNum.h
subtract.o : subtract.c BigNum.h
BigNum.o : BigNum.c BigNum.h

bits : bits.c

clean :
	rm -f core bits add add.o multiply multiply.o divide divide.o subtract subtract.o BigNum.o

all : add multiply divide subtract bits
