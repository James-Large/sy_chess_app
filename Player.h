#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "PieceEnum.h"
#include <string>
#include <SFML/Graphics.hpp>

class Player {
private:
    std::string name;
    bool isWhite;
    int gameScore;
    sf::Time timeLeft; //initialised to start time (e.g if 5|2, timeleft initialised to 5(minutes))
    //is decremented by the turntime clock, incremented each turn by the increment value
    sf::Time increment; //initialised to increment, (e.g if 5|2, increment initialised to 2)
    sf::Clock turnTime;

public:

    Player(const std::string &newName = "", const bool &white = true, const int &score = 0, const sf::Time time = sf::seconds(0.f)):
                            name(newName), isWhite(white), gameScore(score) { }

    std::string& getName() {
        return name;
    }

    void setName(const std::string &newName) {
        name = newName;
    }

    void swapColours() {
        isWhite = (isWhite ? false : true);
    }

    void updateScore() {
        ++gameScore;
    }

};

#endif // PLAYER_H_INCLUDED
