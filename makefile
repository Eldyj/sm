BIN = bin
SRC = src
INCLUDE = $(SRC)/include

CC=gcc
FLAGS=-O2 -g3 -Wall -Wextra -pedantic -I$(INCLUDE)
CCF=$(CC) $(FLAGS)

sm.o:
	$(CCF) -c -o$(BIN)/sm.o $(INCLUDE)/sm.c

strmap.o:
	$(CCF) -c -o$(BIN)/strmap.o $(INCLUDE)/strmap.c

smbin.o: sm.o
	$(CCF) -c -o$(BIN)/smbin.o $(INCLUDE)/smbin.c $(BIN)/sm.o

smasm.o: sm.o strmap.o
	$(CCF) -c -o$(BIN)/smasm.o $(INCLUDE)/smasm.c $(BIN)/sm.o $(BIN)/strmap.o

sme: smbin.o
	$(CCF) -o$(BIN)/sme $(SRC)/sme.c $(BIN)/sm.o $(BIN)/smbin.o

sma: smasm.o
	$(CCF) -o$(BIN)/sma $(SRC)/sma.c $(BIN)/sm.o $(BIN)/smbin.o $(BIN)/smasm.o $(BIN)/strmap.o

all: sma sme
