#include "../include/Word.hpp"

Word::Word(const std::string& word, float startX, float startY, float wordSpeed) 
    : text(word), originalText(word), x(startX), y(startY), 
      progress(0), isActive(false), color(sf::Color::White), speed(wordSpeed) {}

Word createWord(const std::string& word, float startX, float startY, float wordSpeed) {
    return Word(word, startX, startY, wordSpeed);
}

void updateWordPosition(Word& word, float deltaTime) {
    word.y += word.speed * deltaTime;
}

bool isWordOutOfBounds(const Word& word, float screenHeight) {
    return word.y > screenHeight;
}

void setWordActive(Word& word, bool active) {
    word.isActive = active;
    if (active) {
        word.color = sf::Color::Yellow;
    } else {
        word.color = sf::Color::White;
    }
}

void incrementWordProgress(Word& word) {
    if (word.progress < word.originalText.length()) {
        word.progress++;
        if (word.progress < word.text.length()) {
            word.text = word.originalText.substr(word.progress);
        }
    }
}

void resetWordProgress(Word& word) {
    word.progress = 0;
    word.text = word.originalText;
    word.isActive = false;
    word.color = sf::Color::White;
}

bool isWordCompleted(const Word& word) {
    return word.progress >= word.originalText.length();
}