# Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Wall

# Source files and target executable
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = build/blackjack

# Default target
all: $(TARGET)

# Compile source files to object files
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)
