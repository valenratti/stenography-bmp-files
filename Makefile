.PHONY=clean all
INCLUDES=-I /include
CC=gcc
CFLAGS=--std=c11 -pedantic -pedantic-errors -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-sign-compare -Wno-enum-compare -g

TARGET=stegobmp
OBJECTS=$(wildcard *.c)

all: stegobmp
stegano: $(TARGET)

clean:
	rm -f /*.o $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) -lssl -lcrypto -o stegobmp -L/opt/homebrew/opt/openssl@3/lib -I/opt/homebrew/opt/openssl@3/include