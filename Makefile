# Project name
PROJECT = bserializer

# Compiler and flags
CXX = cl
CXXFLAGS = /std:c++20 /O2 /W4 /I include

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TESTS_DIR = tests
LIB_DIR = lib

# Source files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.cpp)

# Object files
CPP_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.obj, $(CPP_SOURCES))
TEST_OBJECTS = $(patsubst $(TESTS_DIR)/%.cpp, $(BUILD_DIR)/%.obj, $(TEST_SOURCES))

# Library output
STATIC_LIB = $(LIB_DIR)/$(PROJECT).lib
SHARED_LIB = $(BIN_DIR)/$(PROJECT).dll
TEST_EXECUTABLE = $(BIN_DIR)/$(PROJECT)_tests.exe

# Default target
all: $(STATIC_LIB) $(SHARED_LIB)

# Compile C++ source files
$(BUILD_DIR)/%.obj: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

# Build static library (.lib)
$(STATIC_LIB): $(CU_OBJECTS) $(CPP_OBJECTS) | $(LIB_DIR)
	lib /OUT:$@ $^

# Build dynamic library (.dll)
$(SHARED_LIB): $(CU_OBJECTS) $(CPP_OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ /Fe$@ /LD $(LDFLAGS)

# Compile tests
$(BUILD_DIR)/%.obj: $(TESTS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

# Link test executable (depends on static library)
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(STATIC_LIB) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ /Fe$@ $(LDFLAGS)

# Run tests
check: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

# Create necessary directories
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(LIB_DIR):
	mkdir $(LIB_DIR)

# Clean build files
clean:
	rd /s /q $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

.PHONY: all clean check