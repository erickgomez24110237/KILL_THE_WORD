#ifndef WORDSPAWNER_HPP
#define WORDSPAWNER_HPP

#include <vector>
#include <string>
#include <random>
#include <SFML/System.hpp>
#include "Word.hpp"

class WordSpawner {
private:
    std::vector<std::string> m_wordList;
    sf::Clock m_spawnClock;
    float m_spawnInterval;
    float m_wordSpeed;
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_wordDist;
    std::uniform_real_distribution<> m_xPosDist;

public:
    WordSpawner() : m_gen(m_rd()), m_wordDist(0, 0), m_xPosDist(0, 1500) {
        m_spawnInterval = 2.0f;
        m_wordSpeed = 85.0f;
        
        m_wordList = {
            "tanque", "casa", "jeremias", "diego", "pantalla", "medicina", "carro",
            "apocalipsis", "pandemia", "laptop", "fernando", "calle", "virus",
            "sangre", "paul", "pelon", "usuario", "cargador", "pistola",
            "soldado", "disparo", "bala", "arma", "zombie", "erick"
        };
        
        m_wordDist = std::uniform_int_distribution<>(0, m_wordList.size() - 1);
    }
    
    void AddWord(const std::string& word) {
        m_wordList.push_back(word);
        m_wordDist = std::uniform_int_distribution<>(0, m_wordList.size() - 1);
    }
    
    bool ShouldSpawnWord() {
        return m_spawnClock.getElapsedTime().asSeconds() >= m_spawnInterval;
    }
    
    Word GenerateWord() {
        if (ShouldSpawnWord()) {
            std::string newWord = m_wordList[m_wordDist(m_gen)];
            float startX = m_xPosDist(m_gen);
            m_spawnClock.restart();
            return Word(newWord, startX, -50, m_wordSpeed);
        }
        return Word("", 0, 0, 0); 
    }
    
    void UpdateDifficulty() {
        if (m_spawnInterval > 0.8f) {
            m_spawnInterval -= 0.001f;
        }
        if (m_wordSpeed < 150.0f) {
            m_wordSpeed += 0.5f;
        }
    }
    
    void ResetSpawner() {
        m_spawnInterval = 2.0f;
        m_wordSpeed = 50.0f;
        m_spawnClock.restart();
    }
    
    float GetSpawnInterval() const {
        return m_spawnInterval;
    }
    
    float GetWordSpeed() const {
        return m_wordSpeed;
    }
};

#endif