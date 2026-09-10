CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I./src -I./include

SRC = src/allocator.c test/test_allocator.c
OBJ = $(SRC:.c=.o)
TARGET = test_allocator

all: $(TARGET)

# Rule to link the executable
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Pattern rule to compile .c files to .o files with INCLUDES
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
