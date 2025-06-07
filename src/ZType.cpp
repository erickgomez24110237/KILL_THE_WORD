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

vector<Word> words;
bool palabras_generadas = false;

void draw_player(sf::RenderWindow& window, sf::Font& fuente, sf::Text& playerText) {
    playerText.setPosition((X_size/2) - 50, Y_size - 150);
    playerText.setFillColor(sf::Color::White);
    window.draw(playerText);
}

void word_generator(sf::RenderWindow& window, sf::Font& fuente) {
    for (const auto& word : words) {
        sf::Text text(word.text, fuente, 30);
        text.setPosition(word.x, word.y);
        text.setFillColor(sf::Color::White);
        window.draw(text);
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

int main() {
    EstadoJuego estado = EstadoJuego::Menu;
    sf::RenderWindow window(sf::VideoMode(X_size, Y_size), "SFML works!");
    sf::Font fuente;

    if (!fuente.loadFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "No se pudo cargar la fuente" << std::endl;
        return -1;
    }

    sf::Text TextNewGame("New game", fuente, 40);
    TextNewGame.setPosition((X_size / 2.f) - 100, Y_size - 300);
    TextNewGame.setFillColor(sf::Color::White);

    sf::Text textPlayer("player", fuente, 40);
    textPlayer.setPosition((X_size / 2.f) - 100, Y_size - 300);
    textPlayer.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (estado == EstadoJuego::Menu &&
                event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (TextNewGame.getGlobalBounds().contains(mousePosF)) {
                    estado = EstadoJuego::Jugando;
                    std::cout << "Juego iniciado" << std::endl;
                }
            }
        }

        window.clear(sf::Color::Black);

        if (estado == EstadoJuego::Menu) {
            draw_menu(window, fuente, TextNewGame);
            
        } else if (estado == EstadoJuego::Jugando) {
            draw_player(window, fuente, textPlayer);

            if (!palabras_generadas) {
                    words.push_back(Word("hola", rand() % X_size, 100));
                    words.push_back(Word("hola 2", rand() % X_size, 100));
                    words.push_back(Word("hola 3", rand() % X_size, 100));
                    palabras_generadas = true;
                }
                
            word_generator(window, fuente);
        }

        window.display();
    }

    return 0;
}
