BIN = oscope_dump
SRCS = $(wildcard *.c)

CFLAGS += -Wall -Wextra -Werror -pedantic

$(BIN): $(SRCS)
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(BIN)
