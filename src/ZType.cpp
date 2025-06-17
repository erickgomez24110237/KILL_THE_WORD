#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

using namespace std;

const int X_size = 1600;
const int Y_size = 1400;

enum class EstadoJuego {
    Menu,
    Jugando
};

struct Word {
    string text;
    float x, y;
    float speed;

    Word(const string& word, float startX, float startY, float wordSpeed)
        : text(word), x(startX), y(startY), speed(wordSpeed) {}
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
    sf::Clock spawnClock;
    sf::Clock gameClock;
    bool palabras_generadas = false;
    EstadoJuego estado = EstadoJuego::Menu;
    float spawnInterval;
    float wordSpeed;
    sf::Text TextNewGame;

    // Generador de números aleatorios
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> wordDist;
    uniform_real_distribution<> xPosDist;

public:
    ZTypeGame(sf::RenderWindow* win) : window(win) {
        if (!font.loadFromFile("assets/OpenSans-Regular.ttf")) {
            cout << "No se pudo cargar la fuente" << endl;
        }

        TextNewGame.setFont(font);
        TextNewGame.setString("New Game");
        TextNewGame.setCharacterSize(40);
        TextNewGame.setPosition((X_size / 2.f) - 100, Y_size - 300);
        TextNewGame.setFillColor(sf::Color::White);

        int score = 0;
        int lives = 3;
        float spawnInterval = 2.0f; // Spawn palabra cada 2 segundos
        wordSpeed = 50.0f; // Pixels por segundo
    }

    void spawnWord() {
        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            string newWord = wordList[wordDist(gen)];
            float startX = xPosDist(gen);
            
            words.emplace_back(newWord, startX, -50, wordSpeed);
            spawnClock.restart();
        }
    }

    void handle_event(sf::Event& event) {
        if (estado == EstadoJuego::Menu && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (TextNewGame.getGlobalBounds().contains(mousePos)) {
                    estado = EstadoJuego::Jugando;
                }
            }
        }

        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }

    void draw() {
        window->clear(sf::Color::Black);

        if (estado == EstadoJuego::Menu) {
            draw_menu();
        } else if (estado == EstadoJuego::Jugando) {
            draw_game();
        }

        window->display();
    }

    void draw_menu() {
        sf::Text titulo("Kill the Word", font, 120);
        titulo.setPosition((X_size / 2.f) - 350, 300);
        titulo.setFillColor(sf::Color::White);

        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (TextNewGame.getGlobalBounds().contains(mousePosF)) {
            TextNewGame.setFillColor(sf::Color::Yellow);
        } else {
            TextNewGame.setFillColor(sf::Color::White);
        }

        window->draw(titulo);
        window->draw(TextNewGame);
    }

    void draw_game() {
        sf::Texture fondo;
        if (!fondo.loadFromFile("assets/background.jpg")) {
            std::cerr << "Imagen no disponible" << std::endl;
            return;
        }

        sf::Sprite spriteBackground;
        spriteBackground.setTexture(fondo);
        spriteBackground.setPosition(0, 0);
        spriteBackground.setScale(2.3f, 2.0f);
        window->draw(spriteBackground);

        draw_player();
        word_generator();
    }

    void draw_player() {
        sf::Texture texturaJugador;
        if (!texturaJugador.loadFromFile("assets/player.png")) {
            std::cerr << "Imagen no disponible" << std::endl;
            return;
        }

        sf::Sprite spriteJugador;
        spriteJugador.setTexture(texturaJugador);
        spriteJugador.setPosition(740, 1100);
        spriteJugador.setScale(0.3f, 0.3f);
        window->draw(spriteJugador);
    }

    void word_generator() {
        if (!palabras_generadas) {
            for (const auto& word : wordList) {
                float x = static_cast<float>(rand() % (X_size - 200));
                float y = static_cast<float>(rand() % (Y_size - 400));
                words.emplace_back(word, x, y);
            }
            palabras_generadas = true;
        }

        for (const auto& word : words) {
            sf::Text text(word.text, font, 30);
            text.setPosition(word.x, word.y);
            text.setFillColor(sf::Color::White);
            window->draw(text);
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(X_size, Y_size), "Kill the Word");
    window.setFramerateLimit(60);

    ZTypeGame game(&window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            game.handle_event(event);
        }

        game.draw();
    }

    return 0;
}
