# Makefile

# Compiler
CC = g++

# Compiler flags
CFLAGS = -c -std=c++14

# Linker flags
LFLAGS = -lsfml-audio -lsfml-system -lsfml-window

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
EXECUTABLE = keyboard 

# Default target
all: $(EXECUTABLE)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

# Link object files into executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

# Clean up object files and executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
