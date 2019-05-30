#ifndef ENDGAMEMESSAGE_H_INCLUDED
#define ENDGAMEMESSAGE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class EndGameMessage {
public:

    std::string state;
    std::string reason;
    sf::Font font;
    sf::Text printedState;
    sf::Text printedReason;
    sf::FloatRect boxBounds;
    sf::Time finish;
    sf::Clock timer;

    EndGameMessage(): font(sf::Font()), printedState(sf::Text()), printedReason(sf::Text()), boxBounds(sf::FloatRect()), timer(sf::Clock()) { }

    void loadMessage(const Enums::GameState &gameState, const sf::FloatRect &bounds) {
        boxBounds = bounds;

        switch (gameState) {
            case Enums::whiteWinMate: {
                state = "White Wins";
                reason = "by Checkmate";
                break;
            }
            case Enums::whiteWinResign: {
                state = "White Wins";
                reason = "by Resignation";
                break;
            }
            case Enums::blackWinMate: {
                state = "Black Wins";
                reason = "by Checkmate";
                break;
            }
            case Enums::blackWinResign: {
                state = "Black Wins";
                reason = "by Resignation";
                break;
            }
            case Enums::drawAgreed: {
                state = "Game Drawn";
                reason = "by Agreement";
                break;
            }
            case Enums::drawInactivity: {
                state = "Game Drawn";
                reason = "Inactivity";
                break;
            }
            case Enums::drawRepetition: {
                state = "Game Drawn";
                reason = "Repetition";
                break;
            }
            case Enums::drawStalemate: {
                state = "Game Drawn";
                reason = "Stalemate";
                break;
            }
            case Enums::drawInsufficient: {
                state = "Game Drawn";
                reason = "Insufficient Material";
                break;
            }
            default: {
                std::cout << "invalid gamestate for endgame message" <<std::endl;
            }
        }

        loadFont();

        printedState.setFont(getFont());
        printedState.setString(state);
        printedState.setCharacterSize(32);
        printedState.setColor(sf::Color::Black);
        printedState.setStyle(sf::Text::Bold);

        printedReason.setFont(getFont());
        printedReason.setString(reason);
        printedReason.setCharacterSize(24);
        printedReason.setColor(sf::Color::Black);

        float stateOffsetX = (boxBounds.width - printedState.getGlobalBounds().width)/2;
        printedState.setPosition(boxBounds.left + stateOffsetX, boxBounds.top + 40.f);

        float reasonOffsetX = (boxBounds.width - printedReason.getGlobalBounds().width)/2;
        printedReason.setPosition(boxBounds.left + reasonOffsetX, boxBounds.top + boxBounds.height - 60.f);


        finish = sf::seconds(5);
        timer.restart();
    }

    void loadFont() {
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cout <<"error loading file" <<std::endl;
        }
    }


    sf::Font& getFont() {
        return font;
    }

    void drawToWindow(sf::RenderWindow &window) {
        window.draw(printedState);
        window.draw(printedReason);
    }

    bool timerUp() {
        return timer.getElapsedTime() > finish;
    }

};

#endif // ENDGAMEMESSAGE_H_INCLUDED
