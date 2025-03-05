-include local_environment.mk

# Project name
PROJECT_NAME = bserializer

# Compiler and flags
CXX = cl
CXXFLAGS = /std:c++20 /O2 /W4 /I include /EHsc
LDFLAGS =

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
STATIC_LIB = $(BIN_DIR)/$(PROJECT_NAME).lib
TEST_EXECUTABLE = $(BIN_DIR)/$(PROJECT_NAME)_tests.exe

# Collect any .lib files in the lib/ directory
LIB_FILES = $(wildcard $(LIB_DIR)/*.lib)

# Default target
all: $(if $(CPP_SOURCES), $(STATIC_LIB) $(SHARED_LIB),) $(TEST_EXECUTABLE)

# Compile C++ source files
$(BUILD_DIR)/%.obj: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

# Build static library (.lib)
$(STATIC_LIB):  $(CPP_OBJECTS) $(LIB_FILES) | $(BIN_DIR)
	lib /OUT:$@ $^

# Compile tests
$(BUILD_DIR)/%.obj: $(TESTS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

# Link test executable (depends on static library)
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(if $(CPP_SOURCES), $(STATIC_LIB),) $(LIB_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ /Fe$@ $(LDFLAGS)

# Run tests
check: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

# Create output directories
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BIN_DIR):
	@mkdir $(BIN_DIR)

# Clean build files
clean:
	if exist $(BUILD_DIR) rd /s /q $(BUILD_DIR);
	if exist $(BIN_DIR) rd /s /q $(BIN_DIR);

.PHONY: all clean check