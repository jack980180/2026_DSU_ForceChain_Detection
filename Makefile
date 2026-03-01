# Compiler and Flags
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Isrc

# Directories
SRC_DIR  = src
TEST_DIR = test
OBJ_DIR  = obj
BIN_DIR  = bin
PLOT_DIR = plot

# Library Source Files (Everything except main)
LIB_SOURCES = $(SRC_DIR)/Vector.cpp \
              $(SRC_DIR)/DisjointSet.cpp \
              $(SRC_DIR)/Particle.cpp \
              $(SRC_DIR)/SimulationBox.cpp \
              $(SRC_DIR)/SimulationDriver.cpp

# Convert .cpp paths to .o paths in the obj directory
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test Sources and Executables
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS    = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(BIN_DIR)/%)

# Main Executable
MAIN_EXEC    = $(BIN_DIR)/sim_engine

# Plot Executable
PLOT_EXEC = $(BIN_DIR)/plotter

# Default Rule: Build the simulation and all tests
all: directories $(MAIN_EXEC) $(TEST_BINS) $(PLOT_EXEC)

# Rule to create necessary directories
# Rule to create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p results
	@mkdir -p img

# Rule to compile Library Object Files (.cpp -> .o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build the Main Simulation Engine
$(MAIN_EXEC): $(SRC_DIR)/main.cpp $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to build Test Executables
$(BIN_DIR)/%: $(TEST_DIR)/%.cpp $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJECTS) -o $@

# Rule to build Plot Executables
$(PLOT_EXEC): $(PLOT_DIR)/plotter.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleanup complete."

# Run the simulation
run: all
	@./$(MAIN_EXEC)

# Run all tests in the bin directory
test: all
	@for test_bin in $(TEST_BINS); do \
		echo "Running $$test_bin..."; \
		$$test_bin; \
		echo ""; \
	done

plot_build: directories $(PLOT_EXEC)

.PHONY: all directories clean test run plot_build