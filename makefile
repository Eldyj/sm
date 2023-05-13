BIN = bin
SRC = src
INCLUDE = $(SRC)/include
CC=gcc
FLAGS=-O2 -Wall -Wextra -pedantic -I$(INCLUDE)
CCF=$(CC) $(FLAGS)

bin_f:
	mkdir $(BIN) -p

sm.o: bin_f
	$(CCF) -c -o$(BIN)/sm.o $(INCLUDE)/sm.c

strmap.o: bin_f
	$(CCF) -c -o$(BIN)/strmap.o $(INCLUDE)/strmap.c

smbin.o: sm.o bin_f
	$(CCF) -c -o$(BIN)/smbin.o $(INCLUDE)/smbin.c $(BIN)/sm.o

smasm.o: sm.o strmap.o bin_f
	$(CCF) -c -o$(BIN)/smasm.o $(INCLUDE)/smasm.c $(BIN)/sm.o $(BIN)/strmap.o

sme: smbin.o bin_f
	$(CCF) -o$(BIN)/sme $(SRC)/sme.c $(BIN)/sm.o $(BIN)/smbin.o

sma: smasm.o smbin.o bin_f
	$(CCF) -o$(BIN)/sma $(SRC)/sma.c $(BIN)/sm.o $(BIN)/smbin.o $(BIN)/smasm.o $(BIN)/strmap.o

all: sma sme

install: all
	cp $(BIN)/sma ~/.local/bin
	cp $(BIN)/sme ~/.local/bin

