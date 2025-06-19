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
    WordSpawner();
    void addWord(const std::string& word);
    bool shouldSpawnWord();
    Word generateWord();
    void updateDifficulty();
    void resetSpawner();
    float getSpawnInterval() const;
    float getWordSpeed() const;
};

// Funciones estructuradas para WordSpawner
WordSpawner createWordSpawner();
void initializeWordList(WordSpawner& spawner);
bool checkSpawnTime(WordSpawner& spawner);
Word spawnRandomWord(WordSpawner& spawner);
void increaseDifficulty(WordSpawner& spawner);
void resetSpawnerSettings(WordSpawner& spawner);

#endif