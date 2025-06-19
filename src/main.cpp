#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "WordSpawner.hpp"
#include "GameState.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
<<<<<<< HEAD
#include "Game.hpp"
=======

// Enum para los estados del juego
enum GameStateEnum {
    MENU,
    PLAYING,
    GAME_OVER
};

// Función para dibujar el menú principal
void drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
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
        spriteBackground.setPosition(0, 0); // posición en la ventana
        spriteBackground.setScale(2.3f, 2.0f);

    window.draw(spriteBackground);

    // Título del juego
    sf::Text titleText("Kill the Words!", fontBold, 120);
    sf::Color customColor(255, 149, 67);
    titleText.setPosition(1600/2 - 400, 200);
    titleText.setFillColor(customColor);
    window.draw(titleText);
    
    // Opción New Game
    sf::Text newGameText("Presiona ENTER para jugar", fontBold, 45);
    newGameText.setPosition(1600/2 - 250, 600);
    newGameText.setFillColor(customColor);
    window.draw(newGameText);
    
    // Instrucciones
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
    
    // Opción salir
    sf::Text exitText("Presiona ESC para salir", fontBold, 30);
    exitText.setPosition(1600/2 - 150, 1000);
    exitText.setFillColor(sf::Color::Yellow);
    window.draw(exitText);
    
    window.display();
}

// Función para manejar input del menú
bool handleMenuInput(const sf::Event& event, GameStateEnum& currentState) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            currentState = PLAYING;
            return true;
        } else if (event.key.code == sf::Keyboard::Escape) {
            return false; // Señal para cerrar el juego
        }
    }
    return true;
}

// Función para actualizar todas las palabras en pantalla
void updateAllWords(std::vector<Word>& words, GameState& gameState, float deltaTime) {
    for (auto it = words.begin(); it != words.end();) {
        it->updatePosition(deltaTime);
        
        // Eliminar palabras que llegaron al fondo
        if (it->isOutOfBounds(1400)) {
            if (!it->isActive || !it->isCompleted()) {
                gameState.loseLife();
            }
            it = words.erase(it);
        } else {
            ++it;
        }
    }
}

// Función para eliminar palabras completadas
void removeCompletedWords(std::vector<Word>& words) {
    words.erase(
        std::remove_if(words.begin(), words.end(), 
            [](const Word& word) { return word.isCompleted(); }),
        words.end()
    );
}

// Función para reiniciar el juego completo
void resetGame(WordSpawner& spawner, GameState& gameState, InputHandler& inputHandler, 
               std::vector<Word>& activeWords) {
    spawner.resetSpawner();
    gameState.resetGame();
    inputHandler.clearInput();
    activeWords.clear();
}
>>>>>>> a54fe4d5547f9c89da89e6320f216d896e503394

int main() {
    Game game;
    
    if (!game.initialize()) {
        std::cerr << "Error inicializando el juego" << std::endl;
        return -1;
    }
    
<<<<<<< HEAD
    game.run();
=======
    // Estado inicial del juego
    GameStateEnum currentGameState = MENU;
    
    // Crear instancias de las clases
    WordSpawner spawner;
    GameState gameState;
    Renderer renderer(&window);
    InputHandler inputHandler;
    
    // Cargar assets del renderer
    if (!renderer.loadAssets()) {
        std::cerr << "Error cargando assets del juego" << std::endl;
        return -1;
    }
    
    // Vector para almacenar las palabras activas
    std::vector<Word> activeWords;
    
    // Clock para delta time
    sf::Clock deltaClock;
    
    // Bucle principal del juego
    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Event event;
        
        // Procesar eventos
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Manejar eventos según el estado del juego
            switch (currentGameState) {
                case MENU:
                    if (!handleMenuInput(event, currentGameState)) {
                        window.close();
                    }
                    break;
                    
                case PLAYING:
                    // Manejar input del juego usando la clase InputHandler
                    inputHandler.handleGameInput(event, gameState, activeWords);
                    
                    // Verificar si el juego terminó
                    if (gameState.isGameOver()) {
                        currentGameState = GAME_OVER;
                    }
                    break;
                    
                case GAME_OVER:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::R) {
                            resetGame(spawner, gameState, inputHandler, activeWords);
                            currentGameState = PLAYING;
                        } else if (event.key.code == sf::Keyboard::Escape) {
                            currentGameState = MENU;
                            resetGame(spawner, gameState, inputHandler, activeWords);
                        }
                    }
                    break;
            }
        }
        
        // Actualizar y renderizar según el estado
        switch (currentGameState) {
            case MENU:
                drawMainMenu(window, menuFont);
                break;
                
            case PLAYING:
                // Generar nuevas palabras
                if (spawner.shouldSpawnWord()) {
                    Word newWord = spawner.generateWord();
                    if (!newWord.text.empty()) {
                        activeWords.push_back(newWord);
                        spawner.updateDifficulty();
                    }
                }
                
                // Actualizar posición de todas las palabras
                updateAllWords(activeWords, gameState, deltaTime);
                
                // Procesar matches de palabras con el input
                inputHandler.processWordMatch(activeWords, gameState);
                
                // Eliminar palabras completadas
                removeCompletedWords(activeWords);
                
                // Renderizar frame del juego
                renderer.renderFullFrame(activeWords, gameState);
                break;
                
            case GAME_OVER:
                // Renderizar pantalla de game over
                renderer.renderFullFrame(activeWords, gameState);
                break;
        }
    }
>>>>>>> a54fe4d5547f9c89da89e6320f216d896e503394
    
    return 0;
}