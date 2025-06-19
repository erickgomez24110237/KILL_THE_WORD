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
    InputHandler();
    void handleTextInput(char character, std::vector<Word>& words); // ← actualizado
    void handleBackspace();
    bool processWordMatch(std::vector<Word>& words, GameState& gameState);
    void clearInput();
    std::string getCurrentInput() const;
    bool handleGameInput(const sf::Event& event, GameState& gameState, std::vector<Word>& words); // ← actualizado
};

// Funciones estructuradas para InputHandler
InputHandler createInputHandler();
void addCharacterToInput(InputHandler& handler, char character);
void removeLastCharacter(InputHandler& handler);
bool checkWordMatches(InputHandler& handler, std::vector<Word>& words, GameState& gameState);
void clearInputBuffer(InputHandler& handler);
std::string getInputBuffer(const InputHandler& handler);

bool processKeyboardInput(InputHandler& handler, const sf::Event& event, GameState& gameState, std::vector<Word>& words);

Word* findMatchingWord(const std::string& input, std::vector<Word>& words);
bool isValidInputCharacter(char character);
void updateWordFromInput(Word& word, const std::string& input, GameState& gameState);

#endif
