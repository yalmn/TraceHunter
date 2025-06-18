# Compiler & Flags
CC = gcc
CFLAGS = -Wall -std=c11 \
  -I./src -I./src/core -I./src/scope -I./src/report -I./src/external

# Optional CFLAGS/LIBS from pkg-config (preferred on Linux/macOS)
PKG_OK := $(shell pkg-config --exists openssl jansson && echo yes || echo no)

ifeq ($(PKG_OK),yes)
  CFLAGS += $(shell pkg-config --cflags openssl jansson)
  LDLIBS += $(shell pkg-config --libs openssl jansson)
else
  # Fallback paths used mainly on macOS with Homebrew
  CFLAGS += -I/usr/local/opt/openssl@3/include -I/usr/local/opt/jansson/include
  LDFLAGS += -L/usr/local/opt/openssl@3/lib -L/usr/local/opt/jansson/lib
  LDLIBS  += -lssl -lcrypto -ljansson
endif

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
