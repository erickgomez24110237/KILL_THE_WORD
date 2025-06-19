# Directorios
SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include

# Archivos
MAIN := $(SRC_DIR)/main.cpp
OUTPUT := $(BIN_DIR)/programa.exe

# Compilador y flags
CXX := g++
CXXFLAGS := -I$(INCLUDE_DIR) -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Regla por defecto
all: execute

# Compilación principal
execute:
	$(CXX) $(CXXFLAGS) $(MAIN) -o $(OUTPUT) $(LIBS)
	.\$(OUTPUT)

<<<<<<< HEAD
=======
<<<<<<< HEAD
=======


>>>>>>> a54fe4d5547f9c89da89e6320f216d896e503394
>>>>>>> 6deabbd9cb655ff02038f699e1f03680827e7c8b
# Ejecutar el programa
run: execute
	.\$(OUTPUT)

# Limpiar binarios
clean:
	del /Q $(BIN_DIR)\*.exe

.PHONY: all clean run execute
