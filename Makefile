CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -pedantic
LDFLAGS :=
LDLIBS  := -lSDL2

TARGET  := chip8
SRCS    := src/main.c src/chip8.c
OBJS    := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c src/chip8.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) roms/IBM\ Logo.ch8

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean run

