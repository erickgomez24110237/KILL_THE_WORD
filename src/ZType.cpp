#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
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
    
    Word(const string& word, float startX, float startY) 
        : text(word), x(startX), y(startY) {}
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
    bool palabras_generadas = false;

public:
    ZTypeGame(sf::RenderWindow* win) : window(win) {
        
        if (!font.loadFromFile("assets/OpenSans-Regular.ttf")) {
            cout << "No se pudo cargar la fuente" << endl;
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
        spriteBackground.setScale(2.3f, 2.0f);

        window->draw(spriteBackground);
        
        draw_UI();
        draw_player();
        word_generator();

        window->display();
    }

    void draw_player() {
        sf::Texture texturaJugador;
            if (!texturaJugador.loadFromFile("assets/player.png")) {
                std::cerr << "No se pudo cargar la imagen del jugador" << std::endl;
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
                float y = static_cast<float>(rand() % (Y_size - 200));
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

    void draw_menu(sf::RenderWindow& window, sf::Font& fuente, sf::Text& TextNewGame) {
        sf::Text titulo("Kill the word", fuente, 120);
        titulo.setPosition((X_size / 2.f) - 350, 300);
        titulo.setFillColor(sf::Color::White);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (TextNewGame.getGlobalBounds().contains(mousePosF)) {
            TextNewGame.setFillColor(sf::Color::Yellow);
        } else {
            TextNewGame.setFillColor(sf::Color::White);
        }

        window.draw(titulo);
        window.draw(TextNewGame);
    }

    void draw_UI(){
        sf::Text TextNewGame("New game", font, 40);
        TextNewGame.setPosition((X_size / 2.f) - 100, Y_size - 300);
        TextNewGame.setFillColor(sf::Color::White);
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
        }
        game.draw();
    }
    
    return 0;
}