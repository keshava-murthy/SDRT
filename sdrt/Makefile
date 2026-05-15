# ============================================================================
# Smart Daily Routine Tracker (SDRT) - Build Configuration
#
# Build:  make
# Run:    make run
# Clean:  make clean
#
# Uses C++17 for: optional, structured bindings, filesystem, etc.
# Uses -pthread for: std::thread, std::mutex, std::condition_variable
# ============================================================================

CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -pthread -I include -I ..
LDFLAGS   = -pthread

SRC_DIR   = src
OBJ_DIR   = obj
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET    = sdrt

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
