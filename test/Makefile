SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TEST=$(SRC:.c=)

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%: $.o
	$(CC) -o $@ $(LDFLAGS) $<

all: $(TEST)
