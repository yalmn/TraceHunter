# Compiler & Flags
CC = gcc
CFLAGS = -Wall -std=c99 \
  -I./src -I./src/core -I./src/scope -I./src/report -I./src/external \
  -I/usr/local/opt/openssl@3/include -I/usr/local/opt/jansson/include

LDFLAGS = -L/usr/local/opt/openssl@3/lib -L/usr/local/opt/jansson/lib
LDLIBS = -lssl -lcrypto -ljansson

# Quell- und Objektdateien
SRC := $(shell find src -name '*.c')
OBJ := $(patsubst src/%.c, build/%.o, $(SRC))

# Ziel
BIN = bin/tracehunter

# Default-Ziel
all: $(BIN)

# Erzeuge Binary
$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

# Kompilierungsregel für .o-Dateien
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -rf build bin

# Nützlich für Debug
print:
	@echo "SRC:" $(SRC)
	@echo "OBJ:" $(OBJ)

.PHONY: all clean print
