#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

//bool getReply() {
//    std::cout << "Start new game? (y/n)";
//    char reply;
//    std::cin >> reply;
//
//    if (reply == 'y') {
//        return true;
//    }
//    return false;
//}

int main() {


//
//    while (getReply()) {
        Game game;
        game.run();
//    }

    return 0;
}


//TODO
//interface:
//   load games, multiple games, etc
//   proper input/output of turns/coords
