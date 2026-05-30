CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC_DIR = src
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Ingredient.cpp $(SRC_DIR)/RecipeBase.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = recipe_manager

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

.PHONY: all clean
