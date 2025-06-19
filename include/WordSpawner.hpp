#ifndef WORDSPAWNER_HPP
#define WORDSPAWNER_HPP

#include <vector>
#include <string>
#include <random>
#include <SFML/System.hpp>
#include "Word.hpp"

class WordSpawner {
private:
    std::vector<std::string> wordList;
    sf::Clock spawnClock;
    float spawnInterval;
    float wordSpeed;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> wordDist;
    std::uniform_real_distribution<> xPosDist;

public:
    WordSpawner() : gen(rd()), wordDist(0, 0), xPosDist(0, 1500) {
        spawnInterval = 2.0f;
        wordSpeed = 85.0f;
        
        wordList = {
            "tanque", "casa", "jeremias", "diego", "pantalla", "medicina", "carro",
            "apocalipsis", "pandemia", "laptop", "fernando", "calle", "virus",
            "sangre", "paul", "pelon", "usuario", "cargador", "pistola",
            "soldado", "disparo", "bala", "arma", "zombie", "erick"
        };
        
        wordDist = std::uniform_int_distribution<>(0, wordList.size() - 1);
    }
    
    void addWord(const std::string& word) {
        wordList.push_back(word);
        wordDist = std::uniform_int_distribution<>(0, wordList.size() - 1);
    }
    
    bool shouldSpawnWord() {
        return spawnClock.getElapsedTime().asSeconds() >= spawnInterval;
    }
    
    Word generateWord() {
        if (shouldSpawnWord()) {
            std::string newWord = wordList[wordDist(gen)];
            float startX = xPosDist(gen);
            spawnClock.restart();
            return Word(newWord, startX, -50, wordSpeed);
        }

        return Word("", 0, 0, 0); 
        return Word("", 0, 0, 0); // Palabra vacía si no debe generar
    }
    
    void updateDifficulty() {
        if (spawnInterval > 0.8f) {
            spawnInterval -= 0.001f;
        }
        if (wordSpeed < 150.0f) {
            wordSpeed += 0.5f;
        }
    }
    
    void resetSpawner() {
        spawnInterval = 2.0f;
        wordSpeed = 50.0f;
        spawnClock.restart();
    }
    
    float getSpawnInterval() const {
        return spawnInterval;
    }
    
    float getWordSpeed() const {
        return wordSpeed;
    }
};

#endif