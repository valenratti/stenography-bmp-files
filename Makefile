.PHONY=clean all
INCLUDES=-I /include
CFLAGS=--std=c11 -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-sign-compare -Wno-enum-compare -g

TARGET=stegano
OBJECTS=$(wildcard *.c)

all: stegano
stegano: $(TARGET)

clean:
	rm -f /*.o $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) -lpthread -o stegano