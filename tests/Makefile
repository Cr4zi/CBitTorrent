CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC_DIR = ../src
OBJ_DIR = ..
TEST_OBJ_DIR = $(OBJ_DIR)/tests
TEST_TARGET = tests_bin

# currently doesn't have main.cpp
# SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp)) # Exclude main.cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard *.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst %.cpp, $(TEST_OBJ_DIR)/%.o, $(TEST_SRCS))

all: $(TEST_TARGET)
	./$(TEST_TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: %.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_TARGET)
	rm -rf $(TEST_OBJ_DIR)

.PHONY: all clean
