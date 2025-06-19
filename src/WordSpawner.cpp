#include "../include/WordSpawner.hpp"

WordSpawner::WordSpawner() : gen(rd()), wordDist(0, 0), xPosDist(0, 1500) {
    spawnInterval = 2.0f;
    wordSpeed = 70.0f;
    
    // Inicializar lista de palabras
    wordList = {
        "hola", "casa", "teclado", "raton", "pantalla", "codigo", "juego",
        "visual", "consola", "carta", "programa", "ventana", "archivo",
        "memoria", "proceso", "sistema", "usuario", "internet", "servidor",
        "cliente", "datos", "algoritmo", "funcion", "variable", "bucle"
    };
    
    wordDist = std::uniform_int_distribution<>(0, wordList.size() - 1);
}

void WordSpawner::addWord(const std::string& word) {
    wordList.push_back(word);
    wordDist = std::uniform_int_distribution<>(0, wordList.size() - 1);
}

bool WordSpawner::shouldSpawnWord() {
    return spawnClock.getElapsedTime().asSeconds() >= spawnInterval;
}

Word WordSpawner::generateWord() {
    if (shouldSpawnWord()) {
        std::string newWord = wordList[wordDist(gen)];
        float startX = xPosDist(gen);
        spawnClock.restart();
        return Word(newWord, startX, -50, wordSpeed);
    }
    return Word("", 0, 0, 0); // Palabra vacía si no debe generar
}

void WordSpawner::updateDifficulty() {
    if (spawnInterval > 0.8f) {
        spawnInterval -= 0.001f;
    }
    if (wordSpeed < 150.0f) {
        wordSpeed += 0.5f;
    }
}

void WordSpawner::resetSpawner() {
    spawnInterval = 2.0f;
    wordSpeed = 50.0f;
    spawnClock.restart();
}

float WordSpawner::getSpawnInterval() const {
    return spawnInterval;
}

float WordSpawner::getWordSpeed() const {
    return wordSpeed;
}

// Implementación de funciones estructuradas
WordSpawner createWordSpawner() {
    return WordSpawner();
}

void initializeWordList(WordSpawner& spawner) {
    // La lista ya se inicializa en el constructor
}

bool checkSpawnTime(WordSpawner& spawner) {
    return spawner.shouldSpawnWord();
}

Word spawnRandomWord(WordSpawner& spawner) {
    return spawner.generateWord();
}

void increaseDifficulty(WordSpawner& spawner) {
    spawner.updateDifficulty();
}

void resetSpawnerSettings(WordSpawner& spawner) {
    spawner.resetSpawner();
}