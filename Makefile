# Copyright (C) 2016 Nathan Phillip Brink
# Distributed under the terms of the MIT license, see LICENSE.md.
.POSIX:
.PHONY: check clean

check: test
	./test
	valgrind --error-exitcode=1 --leak-check=full ./test

test: test.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o '$(@)' '$(<)'
test.o: include/v.h src/c/v.c test.c

clean:
	rm -f *.o test
