SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
BIN := bin/URLShortener
CFLAGS += -Wall -g -Iinclude

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC) | bin/
	$(CC) $(CFLAGS) -o $(BIN) $^

bin/:
	mkdir -p bin

clean:
	$(RM) $(OBJ) $(BIN)