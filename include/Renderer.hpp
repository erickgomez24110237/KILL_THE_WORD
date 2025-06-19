#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "GameState.hpp"

class Renderer {
private:
    sf::RenderWindow* m_window;
    sf::Font m_font;
    sf::Texture m_backgroundTexture;
    sf::Texture m_zombieTexture;
    sf::Texture m_playerTexture;
    sf::Sprite m_backgroundSprite;
    sf::Sprite m_zombieSprite;
    sf::Sprite m_playerSprite;

public:
    Renderer(sf::RenderWindow* win) : m_window(win) {
        if (!m_font.loadFromFile("assets/OpenSans-Regular.ttf")) {
            std::cout << "Error: No se pudo cargar la fuente. Usando fuente por defecto." << std::endl;
        }
    }
    
    bool LoadAssets() {
        bool success = true;
        
        if (!m_backgroundTexture.loadFromFile("assets/background.jpg")) {
            std::cerr << "No se pudo cargar la imagen de fondo" << std::endl;
            success = false;
        } else {
            m_backgroundSprite.setTexture(m_backgroundTexture);
            m_backgroundSprite.setPosition(0, 0);
            m_backgroundSprite.setScale(2.3f, 2.0f);
        }
        
        if (!m_zombieTexture.loadFromFile("assets/zombie2.png")) {
            std::cerr << "No se pudo cargar la imagen del zombie" << std::endl;
            success = false;
        } else {
            m_zombieSprite.setTexture(m_zombieTexture);
            m_zombieSprite.setScale(0.3f, 0.3f);
        }
        
        if (!m_playerTexture.loadFromFile("assets/player.png")) {
            std::cerr << "No se pudo cargar la imagen del jugador" << std::endl;
            success = false;
        } else {
            m_playerSprite.setTexture(m_playerTexture);
            m_playerSprite.setPosition(740, 1100);
            m_playerSprite.setScale(0.3f, 0.3f);
        }
        
        return success;
    }
    
    void Clear() {
        m_window->clear(sf::Color::Black);
    }
    
    void Display() {
        m_window->display();
    }
    
    void DrawBackground() {
        m_window->draw(m_backgroundSprite);
    }
    
    void DrawWords(const std::vector<Word>& words) {
        for (const auto& word : words) {
            sf::Text text(word.Text, m_font, 30);
            text.setPosition(word.X, word.Y);
            text.setFillColor(word.Color);
            m_window->draw(text);
            
            m_zombieSprite.setPosition(word.X, word.Y - 90);
            m_window->draw(m_zombieSprite);
            
            if (word.Progress > 0) {
                std::string progress(word.Progress, '*');
                sf::Text progressText(progress, m_font, 30);
                sf::FloatRect bounds = progressText.getLocalBounds();
                progressText.setPosition(word.X - bounds.width, word.Y);
                progressText.setFillColor(sf::Color::Green);
                m_window->draw(progressText);
            }
        }
    }
    
    void DrawUI(const GameState& gameState) {
        sf::Text scoreText("Score: " + std::to_string(gameState.GetScore()), m_font, 30);
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);
        m_window->draw(scoreText);
        
        sf::Text livesText("Lives: " + std::to_string(gameState.GetLives()), m_font, 30);
        livesText.setPosition(20, 60);
        livesText.setFillColor(sf::Color::White);
        m_window->draw(livesText);
        
        sf::Text timeText("Time: " + std::to_string(gameState.GetElapsedTime()) + "s", m_font, 30);
        timeText.setPosition(20, 100);
        timeText.setFillColor(sf::Color::White);
        m_window->draw(timeText);
    }
    
    void DrawPlayer() {
        m_window->draw(m_playerSprite);
    }
    
    void DrawGameOver(const GameState& gameState) {
        sf::Font fontBold;
        if (!fontBold.loadFromFile("assets/OpenSans-Bold.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente para el menú." << std::endl;
            return;
        }

        sf::Text gameOverText("GAME OVER", fontBold, 120);
        gameOverText.setPosition(1600/2 - 330, 1400/2 - 200);
        gameOverText.setFillColor(sf::Color::Red);
        m_window->draw(gameOverText);
        
        sf::Text finalScore("Puntaje Final: " + std::to_string(gameState.GetScore()), fontBold, 40);
        finalScore.setPosition(1600/2 - 150, 1400/2);
        finalScore.setFillColor(sf::Color::White);
        m_window->draw(finalScore);

        sf::Text restartText("Presiona R para reiniciar o ESC para salir", fontBold, 30);
        restartText.setPosition(1600/2 - 250, 1400/2 + 100);
        restartText.setFillColor(sf::Color::Yellow);
        m_window->draw(restartText);
    }
    
    void RenderFullFrame(const std::vector<Word>& words, const GameState& gameState) {
        Clear();
        DrawBackground();
        
        if (gameState.IsGameOver()) {
            DrawGameOver(gameState);
        } else {
            DrawWords(words);
            DrawUI(gameState);
            DrawPlayer();
        }
        
        Display();
    }
};

#endif