# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin

# Librerías
SFML := -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# Archivos fuente
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
EXE_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.exe,$(CPP_FILES))

# Variables
CXX = g++
CXXFLAGS = -Iinclude -std=c++17
SFML = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/main.cpp src/Renderer.cpp src/Word.cpp src/WordSpawner.cpp src/GameState.cpp src/InputHandler.cpp
BIN = bin/programa2.exe

# Regla para compilar todos los .cpp
all: $(EXE_FILES)

# Regla para compilar cada archivo individual
$(BIN_DIR)/%.exe: $(SRC_DIR)/%.cpp
	g++ $< -o $@ $(SFML)

# Ejecutar el programa principal
run: $(BIN_DIR)/programa.exe
	.\$(BIN_DIR)\programa.exe

# Compilar solo el archivo principal
execute:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(SFML)
	.\$(BIN)

# Limpiar binarios
clean:
	del /Q $(BIN_DIR)\*.exe

.PHONY: all clean run compile
