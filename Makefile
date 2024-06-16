CCC = gcc
CFLAGS = -Iinclude -Wall -g -pthread
SRCDIR = src
OBJDIR = build
BIN = wizard
LIBS = `sdl2-config --cflags --libs`

SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(BIN)

# Link all object files to create the final executable
wizard: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) $(LIBS)

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory and the executable
clean:
	rm -rf $(OBJDIR) $(BIN)

.PHONY: all clean
