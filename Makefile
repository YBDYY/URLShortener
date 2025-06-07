SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
BIN := bin/URLShortener
CFLAGS += -Wall -g -Iinclude

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $^

clean:
	$(RM) $(OBJ) $(BIN)