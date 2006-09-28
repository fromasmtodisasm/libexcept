
CC=gcc
CFLAGS = -Wall

all: test/tester

src/libexcept.so : src/libexcept.c src/libexcept.h
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $^ -o $@

test/tester : test/tester.c src/libexcept.h
	$(CC) -Isrc $(CFLAGS) $(LDFLAGS) $< -o $@ -Lsrc -lexcept

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

