#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include "PieceEnum.h"
#include "Coords.h"
#include <vector>
#include "TextureHolder.h"
#include "Turn.h"

class Board {
public:
    //typedefs for the board
    typedef std::map<Coords, Enums::PieceID> boardType;
    typedef boardType::iterator boardIter;
    typedef boardType::const_iterator boardConstIter;

    //typedefs for the sprite vector
    typedef std::vector<sf::Sprite*> spriteVec;
    typedef spriteVec::iterator spriteVecIter;
    typedef spriteVec::const_iterator spriteVecConstIter;

public: //CHANGE BACK TO PRIVATE ONCE DONE WITH SIZE() TESTING
    boardType board;
    spriteVec pieceSprites;


public:
    ~Board() {
        clearPieceSprites(); //releases the dynamically allocated sprites
    }
    void setup(); //sets up the intiial position on the board
    void populateSprites(const TextureHolder &); //clears and fills pieceSprites with sprites of all the piece on their board with their position set
    void clearPieceSprites(); //releases the dynamically allocated sprites
    void render(sf::RenderWindow &); //draws board and all pieces on it to target window

    bool occupied(const Coords &coords) const { //returns true if there is a piece at the given coordinates
        return board.count(coords);
    }
    Enums::PieceID getPiece(Coords coords) const { //returns the piece, if any, located in the given coordinates
        return (board.find(coords))->second; //occupied() should be checked before calling getPiece() thus valied iterator should be dereferenced
    }

    void movePiece(const Turn &);

};


#endif // BOARD_H_INCLUDED
