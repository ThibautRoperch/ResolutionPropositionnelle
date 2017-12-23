CFLAGS=-Wall -std=c++11 -lpthread -fopenmp
OFLAGS=-O3

SRC_DIR = ./src
BIN_DIR = ./bin

all: initialiser compiler_sources

initialiser:
	mkdir -p $(BIN_DIR)

compiler_sources: $(BIN_DIR)/pigeons.exe $(BIN_DIR)/main.exe

$(BIN_DIR)/pigeons.exe: $(SRC_DIR)/pigeons.cpp
	mpic++ -o $@ $^ $(CFLAGS) $(OFLAGS)

$(BIN_DIR)/main.exe: $(SRC_DIR)/main.cpp
	mpic++ -o $@ $^ $(CFLAGS) $(OFLAGS)

launch-pigeons:
	$(BIN_DIR)/pigeons.exe

time-pigeons:
	time $(BIN_DIR)/pigeons.exe

clean:
	rm -f $(BIN_DIR)/*
