#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <SFML/Graphics.hpp>

struct Word {
    std::string text;
    std::string originalText;
    float x, y;
    int progress;
    bool isActive;
    sf::Color color;
    float speed;
    
    Word(const std::string& word, float startX, float startY, float wordSpeed);
};

// Funciones para manejar Word (programación estructurada)
Word createWord(const std::string& word, float startX, float startY, float wordSpeed);
void updateWordPosition(Word& word, float deltaTime);
bool isWordOutOfBounds(const Word& word, float screenHeight);
void setWordActive(Word& word, bool active);
void incrementWordProgress(Word& word);
void resetWordProgress(Word& word);
bool isWordCompleted(const Word& word);

#endif