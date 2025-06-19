#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <SFML/System.hpp>

class GameState {
private:
    int m_score;
    int m_lives;
    sf::Clock m_gameClock;
    bool m_gameOver;
    std::string m_currentInput;

public:
    GameState() : m_score(0), m_lives(3), m_gameOver(false), m_currentInput("") {}
    
    void AddScore(int points) {
        m_score += points;
    }
    
    void LoseLife() {
        m_lives--;
        if (m_lives <= 0) {
            m_gameOver = true;
        }
    }
    
    void ResetGame() {
        m_score = 0;
        m_lives = 3;
        m_gameOver = false;
        m_currentInput = "";
        m_gameClock.restart();
    }
    
    bool IsGameOver() const {
        return m_gameOver;
    }
    
    int GetScore() const {
        return m_score;
    }
    
    int GetLives() const {
        return m_lives;
    }
    
    int GetElapsedTime() const {
        return static_cast<int>(m_gameClock.getElapsedTime().asSeconds());
    }
    
    std::string GetCurrentInput() const {
        return m_currentInput;
    }
    
    void SetCurrentInput(const std::string& input) {
        m_currentInput = input;
    }
    
    void ClearCurrentInput() {
        m_currentInput = "";
    }
};

#endif