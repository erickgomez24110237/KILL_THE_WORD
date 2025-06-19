#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "GameState.hpp"

class InputHandler {
private:
    std::string currentInput;

public:
    InputHandler() : currentInput("") {}
    
    void handleTextInput(char character, std::vector<Word>& words) {
        if (!isValidInputCharacter(character)) return;

        std::string tentativeInput = currentInput + character;

        for (auto& word : words) {
            if (word.originalText.substr(0, tentativeInput.length()) == tentativeInput) {
                currentInput = tentativeInput; 
                return;
            }
        }
    }
    
    void handleBackspace() {
        if (!currentInput.empty()) {
            currentInput.pop_back();
        }
    }
    
    bool processWordMatch(std::vector<Word>& words, GameState& gameState) {
        if (currentInput.empty()) return false;

        bool foundMatch = false;

        for (auto& word : words) {
            if (word.originalText.substr(0, currentInput.length()) == currentInput) {
                foundMatch = true;

                if (!word.isActive) {
                    word.setActive(true);
                }

<<<<<<< HEAD


=======
<<<<<<< HEAD
=======
                // Palabra completa
>>>>>>> a54fe4d5547f9c89da89e6320f216d896e503394
>>>>>>> 6deabbd9cb655ff02038f699e1f03680827e7c8b
                if (currentInput == word.originalText) {
                    gameState.addScore(word.originalText.length() * 10);
                    word.progress = word.originalText.length();
                    clearInput();
                    return true;
                }

<<<<<<< HEAD
                

=======
<<<<<<< HEAD
                
=======
                // Progreso parcial
>>>>>>> a54fe4d5547f9c89da89e6320f216d896e503394
>>>>>>> 6deabbd9cb655ff02038f699e1f03680827e7c8b
                word.progress = currentInput.length();
                word.text = word.originalText.substr(currentInput.length());
                return true;
            }
        }

        return foundMatch;
    }
    
    void clearInput() {
        currentInput = "";
    }
    
    std::string getCurrentInput() const {
        return currentInput;
    }
    
    bool handleGameInput(const sf::Event& event, GameState& gameState, std::vector<Word>& words) {
        if (event.type == sf::Event::KeyPressed) {
            if (gameState.isGameOver()) {
                if (event.key.code == sf::Keyboard::R) {
                    gameState.resetGame();
                    clearInput();
                    return true;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    return false;
                }
            } else {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    handleBackspace();
                    return true;
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {
            if (!gameState.isGameOver()) {
                char character = static_cast<char>(event.text.unicode);
                handleTextInput(character, words);
                return true;
            }
        }

        return false;
    }

private:
    bool isValidInputCharacter(char character) {
        return (character >= 'a' && character <= 'z') || 
               (character >= 'A' && character <= 'Z') ||
               (character >= '0' && character <= '9');
    }
};

#endif