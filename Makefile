CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TARGET = virtualSHELL
SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "[Link] Building $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "[Done] $(TARGET) built successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "[Compile] $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "[Clean] Removing build files..."
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "[Clean] Done!"

run: $(TARGET)
	@echo "[Run] Starting virtualSHELL..."
	./$(TARGET)

.PHONY: all clean run
