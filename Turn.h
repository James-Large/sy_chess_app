#ifndef TURN_H_INCLUDED
#define TURN_H_INCLUDED

#include "Coords.h"
#include <utility>
#include <stdexcept>
#include <iostream>

class Turn {
public:
    typedef std::pair<Coords, Coords> coordPair;


public:
    coordPair movement;
    Enums::TurnState state;
    Enums::PieceID piece;
    Enums::TurnType moveType;

    Turn(): movement(coordPair()), state(Enums::clearTurn), piece(Enums::wpawn), moveType(Enums::normal) { }
                    //no real relevence to initialising piece with wpawn, just an arbitrary valid value

    const Coords& first() const { //simple function to improve readability   turn.first() instead of turn.movement.first
        return movement.first;
    }
    const Coords& second() const {
        return movement.second;
    }

    void reset() {
        movement = coordPair();
        state = Enums::clearTurn;
        moveType = Enums::normal;
    }

    void push(const Coords &coords) {
        switch (state) {
            case Enums::clearTurn: {
                movement.first = coords;
                state = Enums::sourceDeclared;

                break;
            }
            case Enums::sourceDeclared: {
                movement.second = coords;
                state = Enums::targetDeclared;
                break;
            }
            default: {
                //error
                throw std::runtime_error("third set of coords entered");
            }
        }
    }

    void push(const Coords &coords, const Enums::PieceID sourcePiece) { //overload for the provision of a piece, lazy shortcut,
                                                            //intended only for when source is being declared
        switch (state) {
            case Enums::clearTurn: {
                movement.first = coords;
                state = Enums::sourceDeclared;
                piece = sourcePiece;

                break;
            }
            case Enums::sourceDeclared: {
                movement.second = coords;
                state = Enums::targetDeclared;
                break;
            }
            default: {
                //error
                throw std::runtime_error("third set of coords entered");
            }
        }
    }
};

inline Enums::PieceID findPiece(const int &a) {
    switch (a) {
        case 0: {
            return Enums::wpawn;
        }
        case 1: {
            return Enums::wbishop;
        }
        case 2: {
            return Enums::wrook;
        }
        case 3: {
            return Enums::wknight;
        }
        case 4: {
            return Enums::wqueen;
        }
        case 5: {
            return Enums::wking;
        }
        case 6: {
            return Enums::bpawn;
        }
        case 7: {
            return Enums::bbishop;
        }
        case 8: {
            return Enums::brook;
        }
        case 9: {
            return Enums::bknight;
        }
        case 10: {
            return Enums::bqueen;
        }
        case 11: {
            return Enums::bking;
        }
        default: {
            throw std::runtime_error("Invalid piece");
        }
    }
}

inline Enums::TurnType findMoveType(const char &a) {
    switch (a) {
        case 0: {
            return Enums::normal;
        }
        case 1: {
            return Enums::castle;
        }
        case 2: {
            return Enums::enPassent;
        }
        case 3: {
            return Enums::promoteBishop;
        }
        case 4: {
            return Enums::promoteKnight;
        }
        case 5: {
            return Enums::promoteRook;
        }
        case 6: {
            return Enums::promoteQueen;
        }
        default: {
            throw std::runtime_error("Invalid turntype");
        }
    }
}

inline std::ostream& operator<<(std::ostream &stream, const Turn &turn) {
    //eventually do this properly, as well as input operator
    stream << turn.piece << " " << turn.moveType << " " << turn.first() << " " << turn.second();
    return stream;
}

inline std::istream& operator>>(std::istream &stream, Turn &turn) {
    //do error checking
    if (!stream) {
        return stream;
    }

    int piece, moveType;

    stream >> piece;
    stream >> moveType;

    Coords f, s;
    stream >> f;
    stream >> s;

    turn.reset();
    turn.push(f, findPiece(piece));
    turn.push(s);
    turn.moveType = findMoveType(moveType);

    return stream;
}



#endif // TURN_H_INCLUDED
