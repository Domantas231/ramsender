BIN:=sender
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)
LDFLAGS:=-lblobmsg_json -lubox -lubus -liotp-as-device -largp

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $^

clean:
	$(RM) $(BIN) $(OBJS)