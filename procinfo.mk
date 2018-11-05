TARGETS = procinfo

CFLAGS = -std=gnu99 -g -Wall -Wextra
LDFLAGS = -lutil

all: $(TARGETS)

.PHONY: clean

clean:
	rm -f $(TARGETS) *.o

procinfo: procinfo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
