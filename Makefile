.POSIX:
.PHONY: check dist

check: test
	./test

test: test.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o '$(@)' '$(<)'
test.o: include/pebble-v/v.h src/c/v.c test.c
