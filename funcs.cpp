#include <SFML/Graphics.hpp>
#include "PieceEnum.h"
#include <set>
#include "Coords.h"
#include "Board.h"
#include <iostream>

bool ownsPiece(Enums::PieceID piece, bool white) {
    if (white && piece < 6) {
        return true; //if white's turn and white piece
    }
    else if (!white && piece >= 6) {
        return true; //if black's turn and black piece
    }
    else {
        return false;
    }
}

bool onBoard(const int &x, const int &y) {
    return x>=1 && x<=8 && y>=1 && y<=8;
}

bool onBoard(const sf::Vector2i pos) { //if a given mouse position is within the bounds of the board,
    return pos.x < 400 && pos.y < 400; //return true
}


std::set<Coords> attackingPawn(const Coords &source, const Board &board, const bool &isWhiteTurn) {

    std::set<Coords> results;

    if (isWhiteTurn) { //moving 'up' board (positive y direction)
        //not checking whether the y coord is within bounds, a pawn should never be present on the 8th rank,
        //as the promotion screen shuold have been triggered.

        //Checks a vertical move (cannot take pieces in this fashion)
        Coords fileMove(source.x, source.y+1);
        if (!board.occupied(fileMove)) {
           results.insert(fileMove);
        }

        //if the white pawn is still on the second rank, can move two squares
        if (source.y == 2) {
            Coords startFileMove(source.x, source.y+2);
            if (!board.occupied(startFileMove)) {
               results.insert(startFileMove);
            }
        }

        //Checks a negative diagonal take, can only move in this fashion if a capturable piece is there
        if (source.x-1 >= 1) {
            Coords negDiagMove(source.x-1, source.y+1);
            if (board.occupied(negDiagMove)) {
                Enums::PieceID piece = board.getPiece(negDiagMove);
                if (piece >= 6) { //white's turn, attacking black piece
                    results.insert(negDiagMove);
                }
            }
        }

        //Checks a positive diagonal take, can only move in this fashion if a capturable piece is there
        if (source.x+1 <= 8) {
            Coords posDiagMove(source.x+1, source.y+1);
            if (board.occupied(posDiagMove)) {
                Enums::PieceID piece = board.getPiece(posDiagMove);
                if (piece >= 6) { //white's turn, attacking black piece
                    results.insert(posDiagMove);
                }
            }
        }

    }
    else { //blacks turn, moving 'down' board (negative y direction)

        //Checks a vertical move (cannot take pieces in this fashion)
        Coords fileMove(source.x, source.y-1);
        if (!board.occupied(fileMove)) {
           results.insert(fileMove);
        }

        //if the black pawn is still on the seventh rank, can move two squares
        if (source.y == 7) {
            Coords startFileMove(source.x, source.y-2);
            if (!board.occupied(startFileMove)) {
               results.insert(startFileMove);
            }
        }

        //Checks a negative diagonal take, can only move in this fashion if a capturable piece is there
        if (source.x-1 >= 1) {
            Coords negDiagMove(source.x-1, source.y-1);
            if (board.occupied(negDiagMove)) {
                Enums::PieceID piece = board.getPiece(negDiagMove);
                if (piece < 6) { //white's turn, attacking black piece
                    results.insert(negDiagMove);
                }
            }
        }

        //Checks a positive diagonal take, can only move in this fashion if a capturable piece is there
        if (source.x+1 <= 8) {
            Coords posDiagMove(source.x+1, source.y-1);
            if (board.occupied(posDiagMove)) {
                Enums::PieceID piece = board.getPiece(posDiagMove);
                if (piece < 6) { //white's turn, attacking black piece
                    results.insert(posDiagMove);
                }
            }
        }
    }

    return results;
}

std::set<Coords> attackingRook(const Coords &source, const Board &board, const bool &isWhiteTurn) {

    std::set<Coords> results;

    bool pieceBlocking1 = false;
    for (int x = source.x, y = source.y+1; y <= 8 && !pieceBlocking1; y++) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                results.insert(coords);  //or visa versa
                pieceBlocking1 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking1 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking2 = false;
    for (int x = source.x, y = source.y-1; y >= 1 && !pieceBlocking2; y--) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                results.insert(coords);  //or visa versa
                pieceBlocking2 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking2 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking3 = false;
    for (int x = source.x+1, y = source.y; x <= 8 && !pieceBlocking3; x++) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                results.insert(coords);  //or visa versa
                pieceBlocking3 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking3 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking4 = false;
    for (int x = source.x-1, y = source.y; x >= 1 && !pieceBlocking4; x--) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                results.insert(coords);  //or visa versa
                pieceBlocking4 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking4 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    return results;
}

std::set<Coords> attackingKnight(const Coords &source, const Board &board, const bool &isWhiteTurn) {
    std::set<Coords> results;

    for (int x = -2; x < 3; x++) {
        for (int y = -2; y < 3; y++) {

            if (abs(x) + abs(y) == 3 && onBoard(source.x+x, source.y+y)) { //if legal knight move (exactly 3 squares moved and on board)
                Coords coords(source.x+x, source.y+y);

                if (board.occupied(coords)) {
                    Enums::PieceID piece = board.getPiece(coords);

                    if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                        results.insert(coords);  //or visa versa
                    }
                    else { //white's turn , defending white piece and visa versa black
                    }
                }
                else { //square is empty, and so attackable
                    results.insert(coords);
                }
            }
        }
    }


    return results;
}

std::set<Coords> attackingBishop(const Coords &source, const Board &board, const bool &isWhiteTurn) {
    std::set<Coords> results;

    bool pieceBlocking1 = false;
    for (int x = source.x+1, y = source.y+1; y <= 8 && x <=8 && !pieceBlocking1; x++, y++) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) { //white's turn, attacking black piece
                results.insert(coords);  //or visa versa
                pieceBlocking1 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking1 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking2 = false;
    for (int x = source.x+1, y = source.y-1; y >= 1 && x <= 8 && !pieceBlocking2; x++, y--) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                results.insert(coords);         //or visa versa
                pieceBlocking2 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking2 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking3 = false;
    for (int x = source.x-1, y = source.y+1; x >= 1 && y <= 8 && !pieceBlocking3; x--, y++) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                results.insert(coords);         //or visa versa
                pieceBlocking3 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking3 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    bool pieceBlocking4 = false;
    for (int x = source.x-1, y = source.y-1; x >= 1 && y >= 1 && !pieceBlocking4; x--, y--) {
        Coords coords(x, y);

        if (board.occupied(coords)) {
            Enums::PieceID piece = board.getPiece(coords);

            if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                results.insert(coords);         //or visa versa
                pieceBlocking4 = true;
            }
            else { //white's turn , defending white piece and visa versa black
                pieceBlocking4 = true;
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords);
        }
    }

    return results;
}
std::set<Coords> attackingQueen(const Coords &source, const Board &board, const bool &isWhiteTurn) {
    //queen is the combination of rook and bishop, so just using those two functions
    std::set<Coords> rookMoves = attackingRook(source, board, isWhiteTurn);
    std::set<Coords> bishopMoves = attackingBishop(source, board, isWhiteTurn);

    rookMoves.insert(bishopMoves.begin(), bishopMoves.end());

    return rookMoves;
}

std::set<Coords> attackingKing(const Coords &source, const Board &board, const bool &isWhiteTurn) {
    std::set<Coords> results;

    //Checks ###
    //       # #
    //       xxx
    for (int x = source.x-1, y = source.y-1, i = 0; i < 3; x++, i++) {
        if (onBoard(x, y)) {
            Coords coords(x, y);

            if (board.occupied(coords)) {
                Enums::PieceID piece = board.getPiece(coords);

                if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                    results.insert(coords);         //or visa versa
                }
                else { //white's turn , defending white piece and visa versa black
                }
            }
            else { //square is empty, and so attackable
                results.insert(coords);
            }
        }
    }

    //Checks xxx
    //       # #
    //       ###
    for (int x = source.x-1, y = source.y+1, i = 0; i < 3; x++, i++) {
        if (onBoard(x, y)) {
            Coords coords(x, y);

            if (board.occupied(coords)) {
                Enums::PieceID piece = board.getPiece(coords);

                if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                    results.insert(coords);         //or visa versa
                }
                else { //white's turn , defending white piece and visa versa black
                }
            }
            else { //square is empty, and so attackable
                results.insert(coords);
            }
        }
    }

    //Checks ###
    //       x #
    //       ###
    Coords coords1(source.x-1, source.y);
    if (onBoard(coords1.x, coords1.y)) {
        if (board.occupied(coords1)) {
            Enums::PieceID piece = board.getPiece(coords1);

            if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                results.insert(coords1);         //or visa versa
            }
            else { //white's turn , defending white piece and visa versa black
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords1);
        }
    }

    //Checks ###
    //       # x
    //       ###
    Coords coords2(source.x+1, source.y);
    if (onBoard(coords2.x, coords2.y)) {
        if (coords2.x <= 8 && board.occupied(coords2)) {
            Enums::PieceID piece = board.getPiece(coords2);

            if (!ownsPiece(piece, isWhiteTurn)) {//white's turn, attacking black piece
                results.insert(coords2);         //or visa versa
            }
            else { //white's turn , defending white piece and visa versa black
            }
        }
        else { //square is empty, and so attackable
            results.insert(coords2);
        }
    }

    return results;
}

std::set<Coords> findTargetSquares(const Coords &source, const Board &board, const Enums::PieceID &piece) {
    switch (piece) {
        case 0: {
            return attackingPawn(source, board, true);
            break;
        }
        case 1: {
            return attackingBishop(source, board, true);
            break;
        }
        case 2: {
            return attackingRook(source, board, true);
            break;
        }
        case 3: {
            return attackingKnight(source, board, true);
            break;
        }
        case 4: {
            return attackingQueen(source, board, true);
            break;
        }
        case 5: {
            return attackingKing(source, board, true);
            break;
        }
        case 6: {
            return attackingPawn(source, board, false);
            break;
        }
        case 7: {
            return attackingBishop(source, board, false);
            break;
        }
        case 8: {
            return attackingRook(source, board, false);
            break;
        }
        case 9: {
            return attackingKnight(source, board, false);
            break;
        }
        case 10: {
            return attackingQueen(source, board, false);
            break;
        }
        case 11: {
            return attackingKing(source, board, false);
            break;
        }
        default: {
            //error
            std::cout << "non-piece ID passed to attacking" <<std::endl;
            return std::set<Coords>();
        }
    }
}
