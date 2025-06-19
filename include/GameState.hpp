#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <SFML/System.hpp>

class GameState {
private:
    int score;
    int lives;
    sf::Clock gameClock;
    bool gameOver;
    std::string currentInput;

public:
    GameState() : score(0), lives(3), gameOver(false), currentInput("") {}
    
    void addScore(int points) {
        score += points;
    }
    
    void loseLife() {
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }
    }
    
    void resetGame() {
        score = 0;
        lives = 3;
        gameOver = false;
        currentInput = "";
        gameClock.restart();
    }
    
    bool isGameOver() const {
        return gameOver;
    }
    
    int getScore() const {
        return score;
    }
    
    int getLives() const {
        return lives;
    }
    
    int getElapsedTime() const {
        return static_cast<int>(gameClock.getElapsedTime().asSeconds());
    }
    
    std::string getCurrentInput() const {
        return currentInput;
    }
    
    void setCurrentInput(const std::string& input) {
        currentInput = input;
    }
    
    void clearCurrentInput() {
        currentInput = "";
    }
};

#endif