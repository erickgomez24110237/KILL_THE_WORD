#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <random>
using namespace std;

struct Word {
    string text;
    string originalText;
    float x, y;
    int progress;
    bool isActive;
    sf::Color color;
    float speed;
    
    Word(const string& word, float startX, float startY, float wordSpeed) 
        : text(word), originalText(word), x(startX), y(startY), 
          progress(0), isActive(false), color(sf::Color::White), speed(wordSpeed) {}
};

class ZTypeGame {
private:
    vector<string> wordList = {
        "hola", "casa", "teclado", "raton", "pantalla", "codigo", "juego",
        "visual", "consola", "carta", "programa", "ventana", "archivo",
        "memoria", "proceso", "sistema", "usuario", "internet", "servidor",
        "cliente", "datos", "algoritmo", "funcion", "variable", "bucle"
    };
    
    vector<Word> words;
    sf::RenderWindow* window;
    sf::Font font;
    string currentInput;
    int score;
    int lives;
    sf::Clock spawnClock;
    sf::Clock gameClock;
    float spawnInterval;
    float wordSpeed;
    bool gameOver;
    
    // Generador de números aleatorios
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> wordDist;
    uniform_real_distribution<> xPosDist;

public:
    ZTypeGame(sf::RenderWindow* win) : window(win), gen(rd()), 
                                      wordDist(0, wordList.size() - 1),
                                      xPosDist(0, 750) {  // Reducido a la mitad (1500/2)
        score = 0;
        lives = 3;
        spawnInterval = 2.0f;
        wordSpeed = 25.0f;  // Reducido a la mitad (50/2)
        gameOver = false;
        currentInput = "";
        
        if (!font.loadFromFile("assets/OpenSans-Regular.ttf")) {
            cout << "Error: No se pudo cargar la fuente. Usando fuente por defecto." << endl;
        }
    }
    
    void spawnWord() {
        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            string newWord = wordList[wordDist(gen)];
            float startX = xPosDist(gen);
            
            words.emplace_back(newWord, startX, -25, wordSpeed);  // Reducido a la mitad (50/2)
            spawnClock.restart();
            
            // Incrementar dificultad gradualmente
            if (spawnInterval > 0.8f) {
                spawnInterval -= 0.001f;
            }
            if (wordSpeed < 75.0f) {  // Reducido a la mitad (150/2)
                wordSpeed += 0.5f;
            }
        }
    }
    
    void updateWords(float deltaTime) {
        for (auto it = words.begin(); it != words.end();) {
            it->y += it->speed * deltaTime;
            
            // Eliminar palabras que llegaron al fondo
            if (it->y > 700) {  // Reducido a la mitad (1400/2)
                if (!it->isActive || it->progress < it->originalText.length()) {
                    lives--;
                }
                it = words.erase(it);
            } else {
                ++it;
            }
        }
        
        if (lives <= 0) {
            gameOver = true;
        }
    }
    
    void draw() {
        window->clear(sf::Color::Black);

        sf::Texture fondo;
        if (!fondo.loadFromFile("assets/background.jpg")) {
            std::cerr << "No se pudo cargar la imagen" << std::endl;
            return;
        }

        sf::Sprite spriteBackground;
        spriteBackground.setTexture(fondo);
        spriteBackground.setPosition(0, 0);
        spriteBackground.setScale(1.15f, 1.0f);  // Reducido a la mitad (2.3/2 y 2.0/2)

        window->draw(spriteBackground);
        if (gameOver) {
            drawGameOver();
        } else {
            drawWords();
            drawUI();
            drawPlayer();
        }
        
        window->display();
    }
    
    void drawWords() {
        sf::Texture texturaZombie;
        if (!texturaZombie.loadFromFile("assets/zombie2.png")) {
            std::cerr << "No se pudo cargar la imagen del zombie" << std::endl;
            return;
        }

        sf::Sprite spriteZombie;
        spriteZombie.setTexture(texturaZombie);
        spriteZombie.setScale(0.15f, 0.15f);  // Reducido a la mitad (0.3/2)
        
        for (const auto& word : words) {
            sf::Text text(word.text, font, 15);  // Reducido a la mitad (30/2)
            text.setPosition(word.x, word.y);
            text.setFillColor(word.color);
            window->draw(text);
            
            spriteZombie.setPosition(word.x, word.y - 45);  // Reducido a la mitad (90/2)
            window->draw(spriteZombie);
            
            if (word.progress > 0) {
                string progress(word.progress, '*');
                sf::Text progressText(progress, font, 15);  // Reducido a la mitad (30/2)
                sf::FloatRect bounds = progressText.getLocalBounds();
                progressText.setPosition(word.x - bounds.width, word.y);
                progressText.setFillColor(sf::Color::Green);
                window->draw(progressText);
            }
        }
    }
    
    void drawUI() {
        // Score
        sf::Text scoreText("Score: " + to_string(score), font, 15);  // Reducido a la mitad (30/2)
        scoreText.setPosition(10, 10);  // Reducido a la mitad (20/2)
        scoreText.setFillColor(sf::Color::White);
        window->draw(scoreText);
        
        // Vidas
        sf::Text livesText("Lives: " + to_string(lives), font, 15);  // Reducido a la mitad (30/2)
        livesText.setPosition(10, 30);  // Reducido a la mitad (60/2)
        livesText.setFillColor(sf::Color::White);
        window->draw(livesText);
        
        // Tiempo
        int timeSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        sf::Text timeText("Time: " + to_string(timeSeconds) + "s", font, 15);  // Reducido a la mitad (30/2)
        timeText.setPosition(10, 50);  // Reducido a la mitad (100/2)
        timeText.setFillColor(sf::Color::White);
        window->draw(timeText);
    }
    
    void drawPlayer() {
        sf::Texture texturaJugador;
        if (!texturaJugador.loadFromFile("assets/player.png")) {
            std::cerr << "No se pudo cargar la imagen del jugador" << std::endl;
            return;
        }

        sf::Sprite spriteJugador;
        spriteJugador.setTexture(texturaJugador);
        spriteJugador.setPosition(370, 550);  // Reducido a la mitad (740/2 y 1100/2)
        spriteJugador.setScale(0.15f, 0.15f);  // Reducido a la mitad (0.3/2)

        window->draw(spriteJugador);
    }
    
    void drawGameOver() {
        sf::Text gameOverText("GAME OVER", font, 40);  // Reducido a la mitad (80/2)
        gameOverText.setPosition(400, 350);  // Reducido a la mitad (800-200 y 700-100)
        gameOverText.setFillColor(sf::Color::Red);
        window->draw(gameOverText);
        
        sf::Text finalScore("Final Score: " + to_string(score), font, 20);  // Reducido a la mitad (40/2)
        finalScore.setPosition(400, 400);  // Reducido a la mitad (800-150 y 700/2)
        finalScore.setFillColor(sf::Color::White);
        window->draw(finalScore);
        
        sf::Text restartText("Press R to restart or ESC to exit", font, 15);  // Reducido a la mitad (30/2)
        restartText.setPosition(400, 450);  // Reducido a la mitad (800-250 y 700/2+100)
        restartText.setFillColor(sf::Color::Yellow);
        window->draw(restartText);
    }
    
    void restart() {
        words.clear();
        score = 0;
        lives = 3;
        spawnInterval = 2.0f;
        wordSpeed = 45.0f;  // Reducido a la mitad (90/2)
        gameOver = false;
        currentInput = "";
        spawnClock.restart();
        gameClock.restart();
    }
    
    void update(float deltaTime) {
        if (!gameOver) {
            spawnWord();
            updateWords(deltaTime);
        }
    }
    
    bool isGameOver() const {
        return gameOver;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    sf::RenderWindow window(sf::VideoMode(800, 700), "ZType - Kill the Words!");  // Reducido a la mitad (1600/2 y 1400/2)
    window.setFramerateLimit(60);
    
    ZTypeGame game(&window);
    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (game.isGameOver()) {
                    if (event.key.code == sf::Keyboard::R) {
                        game.restart();
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
            }
        }
        
        game.update(deltaTime);
        game.draw();
    }
    
    return 0;
}