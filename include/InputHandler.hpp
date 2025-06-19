#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Word.hpp"
#include "GameState.hpp"

class InputHandler {
private:
    std::string m_currentInput;

    bool IsValidInputCharacter(char character) {
        return (character >= 'a' && character <= 'z') || 
               (character >= 'A' && character <= 'Z') ||
               (character >= '0' && character <= '9');
    }

public:
    InputHandler() : m_currentInput("") {}
    
    void HandleTextInput(char character, std::vector<Word>& words) {
        if (!IsValidInputCharacter(character)) return;

        std::string tentativeInput = m_currentInput + character;

        for (auto& word : words) {
            if (word.OriginalText.substr(0, tentativeInput.length()) == tentativeInput) {
                m_currentInput = tentativeInput; 
                return;
            }
        }
    }
    
    void HandleBackspace() {
        if (!m_currentInput.empty()) {
            m_currentInput.pop_back();
        }
    }
    
    bool ProcessWordMatch(std::vector<Word>& words, GameState& gameState) {
        if (m_currentInput.empty()) return false;

        bool foundMatch = false;

        for (auto& word : words) {
            if (word.OriginalText.substr(0, m_currentInput.length()) == m_currentInput) {
                foundMatch = true;

                if (!word.IsActive) {
                    word.SetActive(true);
                }

                if (m_currentInput == word.OriginalText) {
                    gameState.AddScore(word.OriginalText.length() * 10);
                    word.Progress = word.OriginalText.length();
                    ClearInput();
                    return true;
                }

                word.Progress = m_currentInput.length();
                word.Text = word.OriginalText.substr(m_currentInput.length());
                return true;
            }
        }

        return foundMatch;
    }
    
    void ClearInput() {
        m_currentInput = "";
    }
    
    std::string GetCurrentInput() const {
        return m_currentInput;
    }
    
    bool HandleGameInput(const sf::Event& event, GameState& gameState, std::vector<Word>& words) {
        if (event.type == sf::Event::KeyPressed) {
            if (gameState.IsGameOver()) {
                if (event.key.code == sf::Keyboard::R) {
                    gameState.ResetGame();
                    ClearInput();
                    return true;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    return false;
                }
            } else {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    HandleBackspace();
                    return true;
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {
            if (!gameState.IsGameOver()) {
                char character = static_cast<char>(event.text.unicode);
                HandleTextInput(character, words);
                return true;
            }
        }

        return false;
    }
};

#endif