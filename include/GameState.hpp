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
    GameState();
    void addScore(int points);
    void loseLife();
    void resetGame();
    bool isGameOver() const;
    int getScore() const;
    int getLives() const;
    int getElapsedTime() const;
    std::string getCurrentInput() const;
    void setCurrentInput(const std::string& input);
    void clearCurrentInput();
};

// Funciones estructuradas para GameState
GameState createGameState();
void incrementScore(GameState& state, int points);
void decrementLives(GameState& state);
void restartGame(GameState& state);
bool checkGameOver(const GameState& state);
int getCurrentScore(const GameState& state);
int getCurrentLives(const GameState& state);
int getGameTime(const GameState& state);
void updateCurrentInput(GameState& state, const std::string& input);
void resetCurrentInput(GameState& state);
std::string getPlayerInput(const GameState& state);

#endif