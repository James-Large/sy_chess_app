#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <set>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Board.h"
#include "Turn.h"
#include "funcs.h"
#include "PromoteBounds.h"
#include "EndGameMessage.h"
#include "Button.h"
#include "Player.h"


class Game {
private:
    sf::RenderWindow window;
    const sf::Time timePerFrame;
    TextureHolder pieceTextures;
    TextureHolder otherTextures;
    Board board;
    Turn turn;
    bool whoseTurn;
    bool drawOffered;
    PromoteBounds promoteBounds;
    Enums::GameState gameState;
    Enums::OverallState progState;
    std::vector<Turn> movesPlayed; //after each move, the turn is push-backed into this vector
    //castling is treated as two seperate turns due to the way it worked out, when playing back from a saved
    //game must specifically check for castling and treat as special case
    //also need to treat enpassents as a special case, the taken pawn must be manually removed since it is not automatically
    //overwritten in the board map
    std::vector<std::pair<Turn, Enums::PieceID> > movesShown; //used to keep track of the moves shown during a loadGame sequence
    //the pieceid corresponding to each turn refers to any pieces that were captured during that turn so taht they can be put
    //back onto the board if a game is 're-winded'

    sf::Sprite boardSprite;
    sf::Sprite highlightBorderSprite;
    sf::Sprite highlightFillSprite1; //used to highlight source of previous move
    sf::Sprite highlightFillSprite2; //used to highlight destination of previous move
    sf::Sprite promoteWhiteSprite;
    sf::Sprite promoteBlackSprite;
    sf::Sprite resignButtonSprite;
    sf::Sprite drawButtonSprite;
    sf::Sprite messageBoxSprite;

    sf::Font standardFont;
    Button newGameButton;
    Button loadGameButton;
    Button leftButton;
    Button rightButton;

    EndGameMessage endMessage;



public:
    Game();
    void run();


private:
    void processEvents();
    void processEventsStartup();
    void processEventsNewGame();
    void processEventsLoadGame();

    void update(sf::Time);
    void updateStartup();
    void updateLiveGame();
    void updateLoadedGame();

    void render();
    void renderStartup();
    void renderLiveGame();
    void renderLoadedGame();

  // void handleInput(sf::Keyboard::Key, bool);
    void loadTextures();
    void loadFont();

    //LEGAL MOVE CHECKS
    bool legalMove() {
        if (castlingRequested()) {
            turn.moveType = Enums::castle;
            return castlingLegal();
        }
        else if (legalEnPassent()) {
            turn.moveType = Enums::enPassent;
            board.board.erase(Coords(turn.second().x, turn.second().y + (whoseTurn ? -1 : 1)));
            return true;
        }
        else {
            return isTargettable() && !wouldPutInCheck(turn, whoseTurn);
        }
    }

    //used to see whether a particular king would be in check after a given move, needs to be called for each king on each move
    //firstly, to see whether the king of the current player would be in check after the move (in which case the move is illegal)
    //secondly, to see whether the king of the opposing player would be in check after the move, and if so, we need to calculate
    //whether the opposing player is in checkmate.
    bool wouldPutInCheck(const Turn &, const bool &);
    bool noLegalMoves(const bool &);
    bool inCheck(const bool &);
    bool isTargettable();
    bool promotionNeeded();

    bool pieceMoved(const Enums::PieceID &, const Coords &);
    bool castlingRequested();
    bool castlingLegal();
    bool castlingThroughCheck(const bool &);
    bool castlingBlocked(const Coords &, const bool &);
    bool legalEnPassent();
    bool startPawnMove(const Turn &);

    bool insufficientMaterial();
    bool sameColour(const Coords &, const Coords &);
    bool getColour(const Coords &);

    bool inactivity();
    bool repetition();

    void processTurn();
    void makeMove();
    void checkGameOver();

    void saveGame();
    void loadGame();

    void startNewGame();
    void loadOldGame();
    void loadPrevMove();
    void loadNextMove();

    bool moveTypePromotion(const Enums::TurnType &);
};


#endif // GAME_H_INCLUDED

