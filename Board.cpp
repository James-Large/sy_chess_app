#include "Board.h"
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include "PieceEnum.h"
#include "Coords.h"
#include <vector>
#include "TextureHolder.h"
#include <iostream>
#include "Turn.h"


void Board::populateSprites(const TextureHolder &textures) { //clears and fills pieceSprites with sprites of all the piece on their board with their position set
    clearPieceSprites();

    for (Board::boardConstIter iter = board.begin(); iter != board.end(); ++iter) {
        pieceSprites.push_back(new sf::Sprite(textures.get(iter->second))); //creates new sprite with texture of the PieceID member of current iter value
        (pieceSprites.back())->setPosition(iter->first.findScreenPos()); //sets position of sprite to screen-equivalent of it's coords
    }
}

void Board::clearPieceSprites() { //clears the pieceSprites vector and deletes the dynamically allocated sprites in it
    for (Board::spriteVecIter iter = pieceSprites.begin(); iter < pieceSprites.end(); ++iter) {
        delete *iter;
    }

    pieceSprites.clear();
}

void Board::render(sf::RenderWindow &window) { //draws board and all pieces on it to target window
    for (Board::spriteVecConstIter iter = pieceSprites.begin(); iter < pieceSprites.end(); ++iter) {
        window.draw(**iter); //iter points to a sprite*, which itself then needs to be dereferenced
    }
}

void Board::setup() { //sets up the intiial position on the board
    for (int i = 1; i < 9; ++i) {
        board.insert(std::make_pair(Coords(i, 2), Enums::wpawn));
    }

    for (int i = 1; i < 9; ++i) {
        board.insert(std::make_pair(Coords(i, 7), Enums::bpawn));
    }

    board.insert(std::make_pair(Coords(1, 1), Enums::wrook));
    board.insert(std::make_pair(Coords(2 ,1), Enums::wknight));
    board.insert(std::make_pair(Coords(3 ,1), Enums::wbishop));
    board.insert(std::make_pair(Coords(4 ,1), Enums::wqueen));
    board.insert(std::make_pair(Coords(5 ,1), Enums::wking));
    board.insert(std::make_pair(Coords(6 ,1), Enums::wbishop));
    board.insert(std::make_pair(Coords(7 ,1), Enums::wknight));
    board.insert(std::make_pair(Coords(8 ,1), Enums::wrook));
    board.insert(std::make_pair(Coords(1 ,8), Enums::brook));
    board.insert(std::make_pair(Coords(2 ,8), Enums::bknight));
    board.insert(std::make_pair(Coords(3 ,8), Enums::bbishop));
    board.insert(std::make_pair(Coords(4 ,8), Enums::bqueen));
    board.insert(std::make_pair(Coords(5 ,8), Enums::bking));
    board.insert(std::make_pair(Coords(6 ,8), Enums::bbishop));
    board.insert(std::make_pair(Coords(7 ,8), Enums::bknight));
    board.insert(std::make_pair(Coords(8 ,8), Enums::brook));
}

void Board::movePiece(const Turn &movement) {
    board[movement.second()] = getPiece(movement.first()); //overwrites existing piece if there is one already there, else creates new elemnt automatically
    board.erase(movement.first());//no piece at source square anymore, delete it
}
