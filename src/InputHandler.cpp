#include "../include/InputHandler.hpp"
#include <algorithm>

InputHandler::InputHandler() {
    currentInput = "";
}

void InputHandler::handleTextInput(char character, std::vector<Word>& words) {
    if (!isValidInputCharacter(character)) return;

    std::string tentativeInput = currentInput + character;

    for (auto& word : words) {
        if (word.originalText.substr(0, tentativeInput.length()) == tentativeInput) {
            currentInput = tentativeInput; 
            return;
        }
    }

}

void InputHandler::handleBackspace() {
    if (!currentInput.empty()) {
        currentInput.pop_back();
    }
}

bool InputHandler::processWordMatch(std::vector<Word>& words, GameState& gameState) {
    if (currentInput.empty()) return false;

    bool foundMatch = false;

    for (auto& word : words) {
        if (word.originalText.substr(0, currentInput.length()) == currentInput) {
            foundMatch = true;

            if (!word.isActive) {
                setWordActive(word, true);
            }

            // Palabra completa
            if (currentInput == word.originalText) {
                incrementScore(gameState, word.originalText.length() * 10);
                word.progress = word.originalText.length();
                clearInput();
                return true;
            }

            // Progreso parcial
            word.progress = currentInput.length();
            word.text = word.originalText.substr(currentInput.length());
            return true;
        }
    }

    return foundMatch;
}


void InputHandler::clearInput() {
    currentInput = "";
}

std::string InputHandler::getCurrentInput() const {
    return currentInput;
}

bool InputHandler::handleGameInput(const sf::Event& event, GameState& gameState, std::vector<Word>& words) {
    if (event.type == sf::Event::KeyPressed) {
        if (gameState.isGameOver()) {
            if (event.key.code == sf::Keyboard::R) {
                restartGame(gameState);
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


// Implementación de funciones estructuradas
InputHandler createInputHandler() {
    return InputHandler();
}

void addCharacterToInput(InputHandler& handler, char character, std::vector<Word>& words) {
    handler.handleTextInput(character, words);
}

void removeLastCharacter(InputHandler& handler) {
    handler.handleBackspace();
}

bool checkWordMatches(InputHandler& handler, std::vector<Word>& words, GameState& gameState) {
    return handler.processWordMatch(words, gameState);
}

void clearInputBuffer(InputHandler& handler) {
    handler.clearInput();
}

std::string getInputBuffer(const InputHandler& handler) {
    return handler.getCurrentInput();
}

bool processKeyboardInput(InputHandler& handler, const sf::Event& event, GameState& gameState, std::vector<Word>& words) {
    return handler.handleGameInput(event, gameState, words);
}

Word* findMatchingWord(const std::string& input, std::vector<Word>& words) {
    for (auto& word : words) {
        if (word.originalText.substr(0, input.length()) == input) {
            return &word;
        }
    }
    return nullptr;
}

bool isValidInputCharacter(char character) {
    return (character >= 'a' && character <= 'z') || 
           (character >= 'A' && character <= 'Z') ||
           (character >= '0' && character <= '9');
}

void updateWordFromInput(Word& word, const std::string& input, GameState& gameState) {
    if (word.originalText.substr(0, input.length()) == input) {
        word.progress = input.length();
        word.text = word.originalText.substr(input.length());
        setWordActive(word, true);
        
        if (input == word.originalText) {
            incrementScore(gameState, word.originalText.length() * 10);
            word.progress = word.originalText.length();
        }
    }
}