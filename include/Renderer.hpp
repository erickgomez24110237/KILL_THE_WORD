#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "GameState.hpp"

class Renderer {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Texture zombieTexture;
    sf::Texture playerTexture;
    sf::Sprite backgroundSprite;
    sf::Sprite zombieSprite;
    sf::Sprite playerSprite;

public:
    Renderer(sf::RenderWindow* win) : window(win) {
        if (!font.loadFromFile("assets/OpenSans-Regular.ttf")) {
            std::cout << "Error: No se pudo cargar la fuente. Usando fuente por defecto." << std::endl;
        }
    }
    
    bool loadAssets() {
        bool success = true;
        
        if (!backgroundTexture.loadFromFile("assets/background.jpg")) {
            std::cerr << "No se pudo cargar la imagen de fondo" << std::endl;
            success = false;
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setPosition(0, 0);
            backgroundSprite.setScale(2.3f, 2.0f);
        }
        
        if (!zombieTexture.loadFromFile("assets/zombie2.png")) {
            std::cerr << "No se pudo cargar la imagen del zombie" << std::endl;
            success = false;
        } else {
            zombieSprite.setTexture(zombieTexture);
            zombieSprite.setScale(0.3f, 0.3f);
        }
        
        if (!playerTexture.loadFromFile("assets/player.png")) {
            std::cerr << "No se pudo cargar la imagen del jugador" << std::endl;
            success = false;
        } else {
            playerSprite.setTexture(playerTexture);
            playerSprite.setPosition(740, 1100);
            playerSprite.setScale(0.3f, 0.3f);
        }
        
        return success;
    }
    
    void clear() {
        window->clear(sf::Color::Black);
    }
    
    void display() {
        window->display();
    }
    
    void drawBackground() {
        window->draw(backgroundSprite);
    }
    
    void drawWords(const std::vector<Word>& words) {
        for (const auto& word : words) {
            sf::Text text(word.text, font, 30);
            text.setPosition(word.x, word.y);
            text.setFillColor(word.color);
            window->draw(text);
            
            zombieSprite.setPosition(word.x, word.y - 90);
            window->draw(zombieSprite);
            
            if (word.progress > 0) {
                std::string progress(word.progress, '*');
                sf::Text progressText(progress, font, 30);
                sf::FloatRect bounds = progressText.getLocalBounds();
                progressText.setPosition(word.x - bounds.width, word.y);
                progressText.setFillColor(sf::Color::Green);
                window->draw(progressText);
            }
        }
    }
    
    void drawUI(const GameState& gameState) {
        sf::Text scoreText("Score: " + std::to_string(gameState.getScore()), font, 30);
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);
        window->draw(scoreText);
        
        sf::Text livesText("Lives: " + std::to_string(gameState.getLives()), font, 30);
        livesText.setPosition(20, 60);
        livesText.setFillColor(sf::Color::White);
        window->draw(livesText);
        
        sf::Text timeText("Time: " + std::to_string(gameState.getElapsedTime()) + "s", font, 30);
        timeText.setPosition(20, 100);
        timeText.setFillColor(sf::Color::White);
        window->draw(timeText);
    }
    
    void drawPlayer() {
        window->draw(playerSprite);
    }
    
    void drawGameOver(const GameState& gameState) {
        sf::Font fontBold;
        if (!fontBold.loadFromFile("assets/OpenSans-Bold.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente para el menú." << std::endl;
            return;
        }

        sf::Text gameOverText("GAME OVER", fontBold, 120);
        gameOverText.setPosition(1600/2 - 330, 1400/2 - 200);
        gameOverText.setFillColor(sf::Color::Red);
        window->draw(gameOverText);
        
        sf::Text finalScore("Puntaje Final: " + std::to_string(gameState.getScore()), fontBold, 40);
        finalScore.setPosition(1600/2 - 150, 1400/2);
        finalScore.setFillColor(sf::Color::White);
        window->draw(finalScore);

        sf::Text restartText("Presiona R para reiniciar o ESC para salir", fontBold, 30);
        restartText.setPosition(1600/2 - 250, 1400/2 + 100);
        restartText.setFillColor(sf::Color::Yellow);
        window->draw(restartText);
    }
    
    void renderFullFrame(const std::vector<Word>& words, const GameState& gameState) {
        clear();
        drawBackground();
        
        if (gameState.isGameOver()) {
            drawGameOver(gameState);
        } else {
            drawWords(words);
            drawUI(gameState);
            drawPlayer();
        }
        
        display();
    }
};

#endif