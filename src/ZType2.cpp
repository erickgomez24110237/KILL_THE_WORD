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
                                      xPosDist(0, 1500) {
        score = 0;
        lives = 3;
        spawnInterval = 2.0f; // Spawn palabra cada 2 segundos
        wordSpeed = 50.0f; // Pixels por segundo
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
            
            words.emplace_back(newWord, startX, -50, wordSpeed);
            spawnClock.restart();
            
            // Incrementar dificultad gradualmente
            if (spawnInterval > 0.8f) {
                spawnInterval -= 0.001f;
            }
            if (wordSpeed < 150.0f) {
                wordSpeed += 0.5f;
            }
        }
    }
    
    void updateWords(float deltaTime) {
        for (auto it = words.begin(); it != words.end();) {
            it->y += it->speed * deltaTime;
            
            // Eliminar palabras que llegaron al fondo
            if (it->y > 1400) {
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
    
    void handleInput(char inputChar) {
        if (gameOver) return;
        
        inputChar = tolower(inputChar);
        bool found = false;
        
        // Buscar palabra activa primero
        for (auto& word : words) {
            if (word.isActive && word.progress < word.originalText.length()) {
                if (word.originalText[word.progress] == inputChar) {
                    word.progress++;
                    word.text = word.originalText.substr(word.progress);
                    word.color = sf::Color::Green;
                    found = true;
                    
                    if (word.progress >= word.originalText.length()) {
                        score += word.originalText.length() * 10;
                        word.color = sf::Color::Blue;
                        // La palabra se eliminará en la próxima actualización
                    }
                    break;
                }
            }
        }
        
        // Si no hay palabra activa, buscar nueva palabra que comience con la letra
        if (!found) {
            for (auto& word : words) {
                if (!word.isActive && word.originalText[0] == inputChar) {
                    word.isActive = true;
                    word.progress = 1;
                    word.text = word.originalText.substr(1);
                    word.color = sf::Color::Yellow;
                    found = true;
                    break;
                }
            }
        }
        
        // Eliminar palabras completadas
        words.erase(remove_if(words.begin(), words.end(), 
                    [](const Word& w) { return w.progress >= w.originalText.length(); }),
                   words.end());
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
        spriteBackground.setPosition(0, 0); // posición en la ventana
        spriteBackground.setScale(2.3f, 2.0f);

        window->draw(spriteBackground);
        if (gameOver) {
            drawGameOver();
        } else {
            drawWords();
            // drawUI();
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
        spriteZombie.setScale(0.3f, 0.3f);
        
        for (const auto& word : words) {
            sf::Text text(word.text, font, 30);
            text.setPosition(word.x, word.y);
            text.setFillColor(word.color);
            window->draw(text);
            
            spriteZombie.setPosition(word.x, word.y - 90);
            window->draw(spriteZombie);
            // Dibujar progreso (asteriscos para letras ya escritas)
            if (word.progress > 0) {
                string progress(word.progress, '*');
                sf::Text progressText(progress, font, 30);
                sf::FloatRect bounds = progressText.getLocalBounds();
                progressText.setPosition(word.x - bounds.width, word.y);
                progressText.setFillColor(sf::Color::Green);
                window->draw(progressText);
            }
        }
    }
    
    void drawUI() {
        // Score
        sf::Text scoreText("Score: " + to_string(score), font, 30);
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);
        window->draw(scoreText);
        
        // Vidas
        sf::Text livesText("Lives: " + to_string(lives), font, 30);
        livesText.setPosition(20, 60);
        livesText.setFillColor(sf::Color::White);
        window->draw(livesText);
        
        // Tiempo
        int timeSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        sf::Text timeText("Time: " + to_string(timeSeconds) + "s", font, 30);
        timeText.setPosition(20, 100);
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
        spriteJugador.setPosition(740, 1100); // posición en la ventana
        spriteJugador.setScale(0.3f, 0.3f);

        window->draw(spriteJugador);
    }
    
    void drawGameOver() {
        sf::Text gameOverText("GAME OVER", font, 80);
        gameOverText.setPosition(1600/2 - 200, 1400/2 - 100);
        gameOverText.setFillColor(sf::Color::Red);
        window->draw(gameOverText);
        
        sf::Text finalScore("Final Score: " + to_string(score), font, 40);
        finalScore.setPosition(1600/2 - 150, 1400/2);
        finalScore.setFillColor(sf::Color::White);
        window->draw(finalScore);
        
        sf::Text restartText("Press R to restart or ESC to exit", font, 30);
        restartText.setPosition(1600/2 - 250, 1400/2 + 100);
        restartText.setFillColor(sf::Color::Yellow);
        window->draw(restartText);
    }
    
    void restart() {
        words.clear();
        score = 0;
        lives = 3;
        spawnInterval = 2.0f;
        wordSpeed = 90.0f;
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
    
    sf::RenderWindow window(sf::VideoMode(1600, 1400), "ZType - Kill the Words!");
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
            
            if (event.type == sf::Event::TextEntered && !game.isGameOver()) {
                if (event.text.unicode < 128 && isalpha(event.text.unicode)) {
                    game.handleInput(static_cast<char>(event.text.unicode));
                }
            }
        }
        
        game.update(deltaTime);
        game.draw();
    }
    
    return 0;
}