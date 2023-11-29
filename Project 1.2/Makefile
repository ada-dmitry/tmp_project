CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRCS = main.c functions.c
OBJS = $(SRCS:.c=.o)
DEPS = declare.h

TARGET = ./out/app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
 $(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
 $(CC) $(CFLAGS) -c $< -o $@

clean:
 rm -f $(TARGET) $(OBJS)