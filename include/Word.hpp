#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Word {
public:
    std::string Text;
    std::string OriginalText;
    float X;
    float Y;
    int Progress;
    bool IsActive;
    sf::Color Color;
    float Speed;
    
    Word(const std::string& word, float startX, float startY, float wordSpeed) 
        : Text(word), OriginalText(word), X(startX), Y(startY), 
          Progress(0), IsActive(false), Color(sf::Color::White), Speed(wordSpeed) {}
    
    void UpdatePosition(float deltaTime) {
        Y += Speed * deltaTime;
    }
    
    bool IsOutOfBounds(float screenHeight) const {
        return Y > screenHeight;
    }
    
    void SetActive(bool active) {
        IsActive = active;
        if (active) {
            Color = sf::Color::Yellow;
        } else {
            Color = sf::Color::White;
        }
    }
    
    void IncrementProgress() {
        if (Progress < OriginalText.length()) {
            Progress++;
            if (Progress < Text.length()) {
                Text = OriginalText.substr(Progress);
            }
        }
    }
    
    void ResetProgress() {
        Progress = 0;
        Text = OriginalText;
        IsActive = false;
        Color = sf::Color::White;
    }
    
    bool IsCompleted() const {
        return Progress >= OriginalText.length();
    }
};

#endif