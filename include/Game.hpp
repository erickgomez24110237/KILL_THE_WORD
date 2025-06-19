#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "WordSpawner.hpp"
#include "GameState.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"

enum GameStateEnum {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
private:
    sf::RenderWindow window;
    sf::Font menuFont;
    
    GameStateEnum currentGameState;
    WordSpawner spawner;
    GameState gameState;
    Renderer renderer;
    InputHandler inputHandler;
    
    std::vector<Word> activeWords;
    sf::Clock deltaClock;
    
    void drawMainMenu() {
        window.clear(sf::Color::Black);
        
        sf::Font fontBold;
        if (!fontBold.loadFromFile("assets/OpenSans-Bold.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente para el menú." << std::endl;
            return;
        }

        sf::Texture fondo;
        if (!fondo.loadFromFile("assets/background.jpg")) {
            std::cerr << "No se pudo cargar la imagen" << std::endl;
            return;
        }

        sf::Sprite spriteBackground;
        spriteBackground.setTexture(fondo);
        spriteBackground.setPosition(0, 0); 
        spriteBackground.setScale(2.3f, 2.0f);

        window.draw(spriteBackground);
        
        sf::Text titleText("Kill the Words!", fontBold, 120);
        sf::Color customColor(255, 149, 67);
        titleText.setPosition(1600/2 - 400, 200);
        titleText.setFillColor(customColor);
        window.draw(titleText);
        
        sf::Text newGameText("Presiona ENTER para jugar", fontBold, 45);
        newGameText.setPosition(1600/2 - 250, 600);
        newGameText.setFillColor(customColor);
        window.draw(newGameText);
        
        sf::Text instructionsTitle("Como jugar:", fontBold, 35);
        instructionsTitle.setPosition(1600/2 - 120, 750);
        instructionsTitle.setFillColor(sf::Color::Yellow);
        window.draw(instructionsTitle);
        
        sf::Text instruction1("- Escribe las palabras para destruir zombies", fontBold, 25);
        instruction1.setPosition(1600/2 - 250, 800);
        instruction1.setFillColor(sf::Color::White);
        window.draw(instruction1);
        
        sf::Text instruction2("- No dejes que los zombies lleguen al fondo", fontBold, 25);
        instruction2.setPosition(1600/2 - 250, 830);
        instruction2.setFillColor(sf::Color::White);
        window.draw(instruction2);
        
        sf::Text instruction3("- Tienes 3 vidas", fontBold, 25);
        instruction3.setPosition(1600/2 - 250, 860);
        instruction3.setFillColor(sf::Color::White);
        window.draw(instruction3);
        
        sf::Text exitText("Presiona ESC para salir", fontBold, 30);
        exitText.setPosition(1600/2 - 150, 1000);
        exitText.setFillColor(sf::Color::Yellow);
        window.draw(exitText);
        
        window.display();
    }
    
    bool handleMenuInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                currentGameState = PLAYING;
                return true;
            } else if (event.key.code == sf::Keyboard::Escape) {
                return false; 
            }
        }
        return true;
    }
    
    void updateAllWords(float deltaTime) {
        for (auto it = activeWords.begin(); it != activeWords.end();) {
            it->updatePosition(deltaTime);
            
            if (it->isOutOfBounds(1400)) {
                if (!it->isActive || !it->isCompleted()) {
                    gameState.loseLife();
                }
                it = activeWords.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void removeCompletedWords() {
        activeWords.erase(
            std::remove_if(activeWords.begin(), activeWords.end(), 
                [](const Word& word) { return word.isCompleted(); }),
            activeWords.end()
        );
    }
    
    void resetGame() {
        spawner.resetSpawner();
        gameState.resetGame();
        inputHandler.clearInput();
        activeWords.clear();
    }
    
    void handleEvents() {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            switch (currentGameState) {
                case MENU:
                    if (!handleMenuInput(event)) {
                        window.close();
                    }
                    break;
                    
                case PLAYING:
                    inputHandler.handleGameInput(event, gameState, activeWords);
                    
                    if (gameState.isGameOver()) {
                        currentGameState = GAME_OVER;
                    }
                    break;
                    
                case GAME_OVER:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::R) {
                            resetGame();
                            currentGameState = PLAYING;
                        } else if (event.key.code == sf::Keyboard::Escape) {
                            currentGameState = MENU;
                            resetGame();
                        }
                    }
                    break;
            }
        }
    }
    
    void update(float deltaTime) {
        switch (currentGameState) {
            case MENU:
                break;
                
            case PLAYING:
                
                if (spawner.shouldSpawnWord()) {
                    Word newWord = spawner.generateWord();
                    if (!newWord.text.empty()) {
                        activeWords.push_back(newWord);
                        spawner.updateDifficulty();
                    }
                }
                
                
                updateAllWords(deltaTime);
                
                
                inputHandler.processWordMatch(activeWords, gameState);
                
                
                removeCompletedWords();
                break;
                
            case GAME_OVER:
                
                break;
        }
    }
    
    void render() {
        switch (currentGameState) {
            case MENU:
                drawMainMenu();
                break;
                
            case PLAYING:
            case GAME_OVER:
                renderer.renderFullFrame(activeWords, gameState);
                break;
        }
    }

public:
    Game() : window(sf::VideoMode(1600, 1400), "ZType - Kill the Words!"), 
             currentGameState(MENU), renderer(&window) {
        window.setFramerateLimit(60);
    }
    
    bool initialize() {
        if (!menuFont.loadFromFile("assets/OpenSans-Regular.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente para el menú." << std::endl;
            return false;
        }
        
        if (!renderer.loadAssets()) {
            std::cerr << "Error cargando assets del juego" << std::endl;
            return false;
        }
        
        return true;
    }
    
    void run() {
        while (window.isOpen()) {
            float deltaTime = deltaClock.restart().asSeconds();
            
            handleEvents();
            update(deltaTime);
            render();
        }
    }
};

#endif