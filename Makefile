SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
BIN := bin/URLShortener
CFLAGS += -Wall -g -Iinclude -fsanitize=address
LDFLAGS += -lssl -lcrypto -lsqlite3 -lmicrohttpd -fsanitize=address

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC) | bin/
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LDFLAGS)

bin/:
	mkdir -p bin

clean:
	$(RM) $(OBJ) $(BIN)
