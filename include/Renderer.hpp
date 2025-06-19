#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
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
    Renderer(sf::RenderWindow* win);
    bool loadAssets();
    void clear();
    void display();
    void drawBackground();
    void drawWords(const std::vector<Word>& words);
    void drawUI(const GameState& gameState);
    void drawPlayer();
    void drawGameOver(const GameState& gameState);
};

// Funciones estructuradas para Renderer
Renderer createRenderer(sf::RenderWindow* window);
bool initializeAssets(Renderer& renderer);
void clearScreen(Renderer& renderer);
void presentScreen(Renderer& renderer);
void renderBackground(Renderer& renderer);
void renderWords(Renderer& renderer, const std::vector<Word>& words);
void renderUI(Renderer& renderer, const GameState& gameState);
void renderPlayer(Renderer& renderer);
void renderGameOverScreen(Renderer& renderer, const GameState& gameState);
void renderFullFrame(Renderer& renderer, const std::vector<Word>& words, const GameState& gameState);

#endif