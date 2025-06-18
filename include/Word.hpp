#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Word {
public:
    std::string text;
    std::string originalText;
    float x, y;
    int progress;
    bool isActive;
    sf::Color color;
    float speed;
    
    Word(const std::string& word, float startX, float startY, float wordSpeed) 
        : text(word), originalText(word), x(startX), y(startY), 
          progress(0), isActive(false), color(sf::Color::White), speed(wordSpeed) {}
    
    void updatePosition(float deltaTime) {
        y += speed * deltaTime;
    }
    
    bool isOutOfBounds(float screenHeight) const {
        return y > screenHeight;
    }
    
    void setActive(bool active) {
        isActive = active;
        if (active) {
            color = sf::Color::Yellow;
        } else {
            color = sf::Color::White;
        }
    }
    
    void incrementProgress() {
        if (progress < originalText.length()) {
            progress++;
            if (progress < text.length()) {
                text = originalText.substr(progress);
            }
        }
    }
    
    void resetProgress() {
        progress = 0;
        text = originalText;
        isActive = false;
        color = sf::Color::White;
    }
    
    bool isCompleted() const {
        return progress >= originalText.length();
    }
};

#endif