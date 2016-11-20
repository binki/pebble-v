.POSIX:
.PHONY: check clean

check: test
	./test

test: test.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o '$(@)' '$(<)'
test.o: include/pebble-v/v.h src/c/v.c test.c

clean:
	rm -f *.o test
