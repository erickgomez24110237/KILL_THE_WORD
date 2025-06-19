#include "../include/GameState.hpp"

GameState::GameState() {
    score = 0;
    lives = 3;
    gameOver = false;
    currentInput = "";
}

void GameState::addScore(int points) {
    score += points;
}

void GameState::loseLife() {
    lives--;
    if (lives <= 0) {
        gameOver = true;
    }
}

void GameState::resetGame() {
    score = 0;
    lives = 3;
    gameOver = false;
    currentInput = "";
    gameClock.restart();
}

bool GameState::isGameOver() const {
    return gameOver;
}

int GameState::getScore() const {
    return score;
}

int GameState::getLives() const {
    return lives;
}

int GameState::getElapsedTime() const {
    return static_cast<int>(gameClock.getElapsedTime().asSeconds());
}

std::string GameState::getCurrentInput() const {
    return currentInput;
}

void GameState::setCurrentInput(const std::string& input) {
    currentInput = input;
}

void GameState::clearCurrentInput() {
    currentInput = "";
}

// Implementación de funciones estructuradas
GameState createGameState() {
    return GameState();
}

void incrementScore(GameState& state, int points) {
    state.addScore(points);
}

void decrementLives(GameState& state) {
    state.loseLife();
}

void restartGame(GameState& state) {
    state.resetGame();
}

bool checkGameOver(const GameState& state) {
    return state.isGameOver();
}

int getCurrentScore(const GameState& state) {
    return state.getScore();
}

int getCurrentLives(const GameState& state) {
    return state.getLives();
}

int getGameTime(const GameState& state) {
    return state.getElapsedTime();
}

void updateCurrentInput(GameState& state, const std::string& input) {
    state.setCurrentInput(input);
}

void resetCurrentInput(GameState& state) {
    state.clearCurrentInput();
}

std::string getPlayerInput(const GameState& state) {
    return state.getCurrentInput();
}