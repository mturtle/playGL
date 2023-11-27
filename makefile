SHELL=C:/Program\ Files/Git/bash.exe
CC=g++
BINARY=glPlay
SRC_DIR=. ./src ./dependencies ./dependencies/GLAD/src
INCLUDE_DIR=-I. -I./dependencies/GLAD/include -I./dependencies/GLFW/include
LIBRARY_DIR=-L./dependencies/GLFW/lib-mingw-w64
LINK_FLAGS=-lglfw3 -lGdi32 -lopengl32
BUILD_DIR=./build
DEP_FLAGS=-MP -MD

SRC_FILES=$(foreach F,$(SRC_DIR),$(wildcard $(F)/*.cpp))
OBJ_FILES=$(patsubst %.cpp,%.o,$(SRC_FILES))
DEP_FILES=$(patsubst %.o,%.d,$(OBJ_FILES))

all: $(BINARY)

$(BINARY): $(OBJ_FILES)
	@echo "Linking"
	$(CC) -o $(BUILD_DIR)/$@ $^ $(LIBRARY_DIR) $(LINK_FLAGS)

%.o: %.cpp
	$(CC) $(DEP_FLAGS) $(INCLUDE_DIR) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OBJ_FILES)
	rm -f $(DEP_FILES)

.PHONY: print_objects
print_objects:
	@echo "Source: $(SRC_FILES)"
	@echo "Object: $(OBJ_FILES)"
	@echo "Dependencies: $(DEP_FILES)"