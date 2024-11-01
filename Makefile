CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC_DIR = src
OBJ_DIR = .
TEST_DIR = tests
TARGET = cbittorrent

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests:
	$(MAKE) -C tests

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	$(MAKE) -C tests clean

.PHONY: all clean
