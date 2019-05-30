#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

#include "Board.h"
#include "Coords.h"
#include "PieceEnum.h"
#include <SFML/Graphics.hpp>
#include <set>

bool ownsPiece(Enums::PieceID, bool);
bool onBoard(const sf::Vector2i);
bool onBoard(const int &, const int &);

//functions that return the set of coords that a given piece can attack
//parent
std::set<Coords> findTargetSquares(const Coords &, const Board &, const Enums::PieceID &);
//and individual piece type functions, bool parameter refers to white/black
//attacking functions DO NOT show pieces that are being defended, however do show pieces that are being attacked
//i.e if a black rook on a1 is being attacked by a white rook on a2, a1 will be one of the returned coords, however
//if the rook on a1 is white, a1 will not be returned. thus, seperate functions are used to check whether a piece is defended
//for king moves
std::set<Coords> attackingPawn(const Coords &, const Board &, const bool &);
std::set<Coords> attackingRook(const Coords &, const Board &, const bool &);
std::set<Coords> attackingKnight(const Coords &, const Board &, const bool &);
std::set<Coords> attackingBishop(const Coords &, const Board &, const bool &);
std::set<Coords> attackingQueen(const Coords &, const Board &, const bool &);
std::set<Coords> attackingKing(const Coords &, const Board &, const bool &);

#endif // FUNCS_H_INCLUDED
