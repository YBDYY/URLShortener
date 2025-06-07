SRC:=$(wildcard *.c)
BIN:=URLShortener
OBJ:=$(SRC:.c=.o)
CFLAGS +=-Wall

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $^

clean:
	$(RM) $(OBJ) $(BIN)