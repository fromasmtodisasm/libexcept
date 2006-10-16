
CC=gcc
CFLAGS = -Wall

all: test/test-libexcept

src/libexcept.so : src/libexcept.c src/libexcept.h
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $^ -o $@

test/test-libexcept : test/test-libexcept.c src/libexcept.h src/libexcept.so
	$(CC) -Isrc $(CFLAGS) $(LDFLAGS) $< -o $@ -Lsrc -lexcept

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

