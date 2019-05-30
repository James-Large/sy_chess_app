#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Turn.h"
#include <iostream>
#include "funcs.h"
#include <utility>
#include "Board.h"
#include <fstream>
#include <sstream>

Game::Game(): window(sf::VideoMode(800, 600), "Chess"), timePerFrame(sf::seconds(1.f / 60.f)),
              pieceTextures(TextureHolder()), otherTextures(TextureHolder()), board(Board()), turn(Turn()),
              whoseTurn(true), drawOffered(false), promoteBounds(PromoteBounds()), gameState(Enums::live), progState(Enums::startup),
              movesPlayed(std::vector<Turn>()), boardSprite(sf::Sprite()), highlightBorderSprite(sf::Sprite()),
              highlightFillSprite1(sf::Sprite()), highlightFillSprite2(sf::Sprite()), promoteWhiteSprite(sf::Sprite()),
              promoteBlackSprite(sf::Sprite()), resignButtonSprite(sf::Sprite()), drawButtonSprite(sf::Sprite()), standardFont(sf::Font()),
              newGameButton(Button()), loadGameButton(Button()), leftButton(Button()), rightButton(Button()), endMessage(EndGameMessage()) {
    loadTextures(); //may later provide options to play with different piece sets, but for now this will do
    boardSprite.setTexture(otherTextures.get(Enums::board));
    highlightBorderSprite.setTexture(otherTextures.get(Enums::highlightBorder));

    highlightFillSprite1.setTexture(otherTextures.get(Enums::highlightFill));
    highlightFillSprite2.setTexture(otherTextures.get(Enums::highlightFill));

    promoteWhiteSprite.setTexture(otherTextures.get(Enums::promoteWhite));
    promoteWhiteSprite.setPosition(100.f, 100.f);

    promoteBlackSprite.setTexture(otherTextures.get(Enums::promoteBlack));
    promoteBlackSprite.setPosition(100.f, 100.f);

    resignButtonSprite.setTexture(otherTextures.get(Enums::resignButton));
    resignButtonSprite.setPosition(425.f, 25.f);

    drawButtonSprite.setTexture(otherTextures.get(Enums::drawButton));
    drawButtonSprite.setPosition(425.f, 125.f);

    messageBoxSprite.setTexture(otherTextures.get(Enums::messageBox));
    messageBoxSprite.setPosition(75.f, 125.f);

    loadFont();
    newGameButton.build(standardFont, otherTextures.get(Enums::button), sf::Vector2f(425.f, 25.f), "New Game", 24);
    loadGameButton.build(standardFont, otherTextures.get(Enums::button), sf::Vector2f(425.f, 125.f), "Load Last", 24);
    leftButton.build(standardFont, otherTextures.get(Enums::button), sf::Vector2f(33.3f, 425.f), "<=", 24);
    rightButton.build(standardFont, otherTextures.get(Enums::button), sf::Vector2f(216.6f, 425.f), "=>", 24);
}

void Game::loadFont() {
    if (!standardFont.loadFromFile("resources/arial.ttf")) {
        //error
        std::cout << "unable to load font file" <<std::endl;
    }
}

void Game::run() {
    whoseTurn = true;
    drawOffered = false;
    movesPlayed.clear();
    board = Board();
    board.setup();

    sf::Clock clock; //controls to ensure consistent frame rate
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (window.isOpen()) {
        processEvents();

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }

        render();
    }
}

void Game::loadTextures() { //self-explanatory
    pieceTextures.load(Enums::wpawn, "resources/wpawn.png");
    pieceTextures.load(Enums::wbishop, "resources/wbishop.png");
    pieceTextures.load(Enums::wknight, "resources/wknight.png");
    pieceTextures.load(Enums::wrook, "resources/wrook.png");
    pieceTextures.load(Enums::wqueen, "resources/wqueen.png");
    pieceTextures.load(Enums::wking, "resources/wking.png");
    pieceTextures.load(Enums::bpawn, "resources/bpawn.png");
    pieceTextures.load(Enums::bbishop, "resources/bbishop.png");
    pieceTextures.load(Enums::brook, "resources/brook.png");
    pieceTextures.load(Enums::bqueen, "resources/bqueen.png");
    pieceTextures.load(Enums::bknight, "resources/bknight.png");
    pieceTextures.load(Enums::bking, "resources/bking.png");

    otherTextures.load(Enums::board, "resources/board.jpg");
    otherTextures.load(Enums::highlightBorder, "resources/highlightBorder.png");
    otherTextures.load(Enums::highlightFill, "resources/highlightFill.png");
    otherTextures.load(Enums::promoteWhite, "resources/promotionWhite.png");
    otherTextures.load(Enums::promoteBlack, "resources/promotionBlack.png");
    otherTextures.load(Enums::resignButton, "resources/resign.png");
    otherTextures.load(Enums::drawButton, "resources/draw.png");
    otherTextures.load(Enums::messageBox, "resources/messageBox.png");
    otherTextures.load(Enums::button, "resources/button.png");
}

void Game::processEvents() { //to be completed, check for mouse clicks
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::MouseButtonPressed: {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (onBoard(sf::Mouse::getPosition(window))) { //if click is within bounds of the board
                        if (progState == Enums::liveGame) {
                            if (gameState == Enums::promotion) {
                                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                                if (promoteBounds.overallBox.contains(mousePos)) {
                                    if (promoteBounds.bishopBox.contains(mousePos)) {
                                        board.board[turn.first()] = whoseTurn ? Enums::wbishop : Enums::bbishop;
                                        turn.moveType = Enums::promoteBishop;
                                    }
                                    else if (promoteBounds.knightBox.contains(mousePos)) {
                                        board.board[turn.first()] = whoseTurn ? Enums::wknight : Enums::bknight;
                                        turn.moveType = Enums::promoteKnight;
                                    }
                                    else if (promoteBounds.rookBox.contains(mousePos)) {
                                        board.board[turn.first()] = whoseTurn ? Enums::wrook : Enums::brook;
                                        turn.moveType = Enums::promoteRook;
                                    }
                                    else if (promoteBounds.queenBox.contains(mousePos)) { //queen clicked
                                        board.board[turn.first()] = whoseTurn ? Enums::wqueen : Enums::bqueen;
                                        turn.moveType = Enums::promoteQueen;
                                    }

                                    gameState = Enums::live; //promotion screen processed, go back to live play
                                }
                                else {
                                    //did not click on any particular piece, but somewhere else, ignore.
                                }
                            }
                            else {
                                Coords coords(sf::Mouse::getPosition(window));

                                if (gameState == Enums::live) {
                                    switch (turn.state) {
                                        case Enums::clearTurn: {
                                            if (board.occupied(coords) && ownsPiece(board.getPiece(coords), whoseTurn)) {
                                                turn.push(coords, board.getPiece(coords));
                                                highlightBorderSprite.setPosition(coords.findScreenPos());
                                            }
                                            break;
                                        }
                                        case Enums::sourceDeclared: {
                                            turn.push(coords);
                                            break;
                                        }
                                        default: {
                                            //if target declared, then game logic has yet to update, do nothing
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else { //click was not on board
                        if (progState == Enums::startup) {
                            //very bad code, clicked sets the internal click state to true, which will be acted upon
                            //later in the update function. the 'clicked' function should be renamed, suggests more is done
                            //than just checking whether it was clicked.
                            newGameButton.clicked(sf::Mouse::getPosition(window));
                            loadGameButton.clicked(sf::Mouse::getPosition(window));
                        }
                        else if(progState == Enums::liveGame) {
                            if (gameState == Enums::live) {
                                sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
                                sf::Vector2f mousePosFloat(float(mousePosInt.x), float(mousePosInt.y));

                                if (resignButtonSprite.getGlobalBounds().contains(mousePosFloat)) {
                                    gameState = (whoseTurn ? Enums::blackWinResign : Enums::whiteWinResign); //if white's turn,
                                                                            //white has pressed resign button, so black wins
                                }
                                else if (drawButtonSprite.getGlobalBounds().contains(mousePosFloat)) {
                                    if (drawOffered) {
                                        gameState = Enums::drawAgreed;
                                    }
                                    else {
                                        if (movesPlayed.size() >= 100) { //50 moves for each player
                                            if (inactivity()) {
                                                gameState = Enums::drawInactivity;
                                            }
                                        }
                                        if (movesPlayed.size() >= 6) {
                                            if (repetition()) {
                                                gameState = Enums::drawRepetition;
                                            }
                                        }
                                        drawOffered = true;
                                    }
                                }
                            }
                        }
                        else { //progstate == Enums::loadedGame
                            //very bad code, clicked sets the internal click state to true, which will be acted upon
                            //later in the update function. the 'clicked' function should be renamed, suggests more is done
                            //than just checking whether it was clicked.
                            leftButton.clicked(sf::Mouse::getPosition(window));
                            rightButton.clicked(sf::Mouse::getPosition(window));
                            newGameButton.clicked(sf::Mouse::getPosition(window));
                        }
                    }
                }
                break;
            }
            default: {
                break; //do nothing
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    switch (progState) {
        case Enums::liveGame: {
            updateLiveGame();
            break;
        } //end progState case liveGame
        case Enums::startup: {
            updateStartup();
            break;
        }
        case Enums::loadedGame: {
            updateLoadedGame();
            break;
        }
    }
}

void Game::updateLiveGame() {
    switch (gameState) {
        case Enums::live: {
            if (turn.state == Enums::targetDeclared) {
                if (legalMove()) {
                    processTurn();
                }
                else {//illegal move
                    turn.reset();
                }
            }
            break;
        }
        case Enums::whiteWinResign:
        case Enums::whiteWinMate:
        case Enums::blackWinResign:
        case Enums::blackWinMate:
        case Enums::drawRepetition:
        case Enums::drawInactivity:
        case Enums::drawAgreed:
        case Enums::drawInsufficient:
        case Enums::drawStalemate: {
            saveGame();

            endMessage.loadMessage(gameState, messageBoxSprite.getGlobalBounds());
            gameState = Enums::dead;
            break;
        }
        case Enums::dead: {
            if (endMessage.timerUp()) {
                progState = Enums::startup;
            }
            break;
        }
        case Enums::promotion:
        default: {
            break;
        }
    } //end switch (gameState)
}

void Game::updateLoadedGame() {
    if (leftButton.hasBeenClicked()) {
        if (!movesShown.empty()) { //if not at start of game
            loadPrevMove();
        }

        leftButton.reset();
    }
    if (rightButton.hasBeenClicked()) {;
        if (movesShown.size() < movesPlayed.size()) { //if not at end of game
            loadNextMove();
        }

        rightButton.reset();
    }
    if (newGameButton.hasBeenClicked()) {
        startNewGame();
        newGameButton.reset();
    }
}

void Game::updateStartup() {
    if (newGameButton.hasBeenClicked()) {
        startNewGame();
        newGameButton.reset();
    }
    else if (loadGameButton.hasBeenClicked()) {
        loadOldGame();
        loadGameButton.reset();
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(boardSprite);

    switch (progState) {
        case Enums::startup: {
            renderStartup();
            break;
        }
        case Enums::liveGame: {
            renderLiveGame();
            break;
        }
        case Enums::loadedGame: {
            renderLoadedGame();
            break;
        }
    }

    window.display();
}

void Game::renderStartup() {
    newGameButton.render(window);
    loadGameButton.render(window);
}

void Game::renderLiveGame() {
    board.populateSprites(pieceTextures);

    if (!movesPlayed.empty()) {
        highlightFillSprite1.setPosition(movesPlayed.back().first().findScreenPos());
        highlightFillSprite2.setPosition(movesPlayed.back().second().findScreenPos());

        window.draw(highlightFillSprite1);
        window.draw(highlightFillSprite2);
    }

    board.render(window);
    if (turn.state == Enums::sourceDeclared) {
        window.draw(highlightBorderSprite);
    }

    if (gameState == Enums::promotion) {
        if (whoseTurn) {  //white's turn, really must change this identifier
            window.draw(promoteWhiteSprite);
        }
        else {
            window.draw(promoteBlackSprite);
        }
    }

    if (gameState == Enums::live || gameState == Enums::promotion) {
        window.draw(resignButtonSprite);
        window.draw(drawButtonSprite);
    }
    else {
        window.draw(messageBoxSprite);
        endMessage.drawToWindow(window);
    }
}

void Game::renderLoadedGame() {
    if (!movesShown.empty()) {
        highlightFillSprite1.setPosition(movesShown.back().first.first().findScreenPos());
        highlightFillSprite2.setPosition(movesShown.back().first.second().findScreenPos());

        window.draw(highlightFillSprite1);
        window.draw(highlightFillSprite2);
    }

    board.populateSprites(pieceTextures);
    board.render(window);

    rightButton.render(window);
    leftButton.render(window);
    newGameButton.render(window);
}

bool Game::isTargettable() {
    std::set<Coords> attackableSquares = findTargetSquares(turn.first(), board, turn.piece);
    //findTargetSquares declared/defined in funcs.h / funcs.cpp for god only knows what reason

    if (attackableSquares.count(turn.second())) {
        return true;
    }
    else {
        return false;
    }
}

//used to see whether a particular king would be in check after a given move, needs to be called for each king on each move
//firstly, to see whether the king of the current player would be in check after the move (in which case the move is illegal)
//secondly, to see whether the king of the opposing player would be in check after the move, and if so, we need to calculate
//whether the opposing player is in checkmate.
bool Game::wouldPutInCheck(const Turn &givenTurn, const bool &whitesTurn) {

    //findTargetSquares() will have already been called, and so move must be valid
    bool wasOccupied = false;
    std::pair<Coords, Enums::PieceID> takenPiece;

    if (board.occupied(givenTurn.second())) {
        wasOccupied = true;

        Board::boardIter it = board.board.find(givenTurn.second());

        if (it != board.board.end()) {
            takenPiece = *it;
        }

    }
    board.movePiece(givenTurn);

    bool wouldPutInCheck;

    if (inCheck(whitesTurn)) {
        wouldPutInCheck = true;
    }
    else {
        wouldPutInCheck = false;
    }

    //moving the piece back to it's original place
    Turn reverseTurn;
    reverseTurn.push(givenTurn.second(), givenTurn.piece);
    reverseTurn.push(givenTurn.first());
    board.movePiece(reverseTurn);
    if (wasOccupied) {
        board.board.insert(takenPiece);
    }

    return wouldPutInCheck;
}

bool Game::noLegalMoves(const bool &white) {
    //finds all potential moves, goes through each
    //to see if any results in the player not being in check

    std::vector<Turn> possibleMoves;

    for (Board::boardConstIter iter = board.board.begin(); iter != board.board.end(); iter++) {
        if (white) {
            if (iter->second < 6) {
                std::set<Coords> attackableSquares = findTargetSquares(iter->first, board, iter->second);
                for (std::set<Coords>::iterator iter2 = attackableSquares.begin(); iter2 != attackableSquares.end(); iter2++) {
                    Turn tempTurn;
                    tempTurn.push(iter->first, iter->second);
                    tempTurn.push(*iter2);
                    possibleMoves.push_back(tempTurn);
                }
            }
        }
        else {
            if (iter->second >= 6) {
                std::set<Coords> attackableSquares = findTargetSquares(iter->first, board, iter->second);
                for (std::set<Coords>::iterator iter2 = attackableSquares.begin(); iter2 != attackableSquares.end(); iter2++) {
                    Turn tempTurn;
                    tempTurn.push(iter->first, iter->second);
                    tempTurn.push(*iter2);
                    possibleMoves.push_back(tempTurn);
                }
            }
        }
    }

    bool result = true; //default: there are no legal moves
    //if legal move found, change to false

    //all candidate moves found, now to see if any result in the player not being in check
    for (std::vector<Turn>::iterator iter = possibleMoves.begin(); iter != possibleMoves.end(); iter++) {
        if (!wouldPutInCheck(*iter, white)) {
            result = false;
        }

    }
    return result;
}

//sees whether a given player is in check
bool Game::inCheck(const bool &white) {
    bool result = false;

    Coords kingLocation;
    //finding the location of the king we want to find out about
    //and finding all of the attackable squares of the opposite coloured pieces
    std::set<Coords> allAttackedSquares;



    for (Board::boardConstIter iter = board.board.begin(); iter != board.board.end(); iter++) {
        if (white) {
            if (iter->second == Enums::wking) {//looking for white king
                kingLocation = iter->first;
            }
            else if (iter->second >= 6) {//looking for squares attacked by black
                std::set<Coords> attackedSquares = findTargetSquares(iter->first, board, iter->second);
                allAttackedSquares.insert(attackedSquares.begin(), attackedSquares.end());
            }
        }
        else {
            if (iter->second == Enums::bking) {//looking for black king
                kingLocation = iter->first;
            }
            else if (iter->second < 6) {//looking for squares attacked by white
                std::set<Coords> attackedSquares = findTargetSquares(iter->first, board, iter->second);
                allAttackedSquares.insert(attackedSquares.begin(), attackedSquares.end());
            }
        }
    }

    //if the current location of the king is one of the squares being attacked by the opposition
    //then this king is currently in check
    if (allAttackedSquares.count(kingLocation)) {
        result = true;
    }
    else {
        result = false;
    }

    return result;
}

bool Game::promotionNeeded() {
    if (board.getPiece(turn.first()) == Enums::wpawn && turn.second().y == 8) {
        return true;
    }
    else if (board.getPiece(turn.first()) == Enums::bpawn && turn.second().y == 1) {
        return true;
    }

    return false;
}

bool Game::castlingRequested() {
    if (turn.first() == Coords(5, 1) && turn.piece == Enums::wking) {
        if (turn.second() == Coords(7, 1) || turn.second() == Coords(3, 1)) {
            return true;
        }
    }
    if (turn.first() == Coords(5, 8) && turn.piece == Enums::bking) {
        if (turn.second() == Coords(7, 8) || turn.second() == Coords(3, 8)) {
            return true;
        }
    }
    return false;
}

bool Game::pieceMoved(const Enums::PieceID &piece, const Coords &startLocation) {
    for (std::vector<Turn>::const_iterator iter = movesPlayed.begin(); iter < movesPlayed.end(); iter++) {
        if (iter->piece == piece && turn.first() == startLocation) {
            return true;
        }
    }
    return false;
}


bool Game::castlingLegal() {
    //castling legal if:
    //  neither king nor rook has moved yet
    //  clear path between king and rook
    //  not already in nor moving through check

    bool kingSideCastle = turn.first().x < turn.second().x;
    Enums::PieceID rook = (whoseTurn ? Enums::wrook : Enums::brook);
    Coords rookStartLoc((turn.first().x < turn.second().x ? 8 : 1), (whoseTurn ? 1 : 8));


    if (pieceMoved(turn.piece, turn.first()) || pieceMoved(rook, rookStartLoc)) {
        return false;
    } //if here, neither rook nor king has moved

    if (castlingBlocked(rookStartLoc, kingSideCastle)) {
        return false;
    }//if here, nothing in the way

    if (inCheck(whoseTurn) || castlingThroughCheck(kingSideCastle)) {
        return false;
    }//if here, is not already and will not be passing through check
    //all test passed, castling is legal

    return true;
}

bool Game::castlingThroughCheck(const bool &kingSideCastle) {
    if (kingSideCastle) {
        for (int x = turn.first().x + 1; x <= turn.second().x; x++) {
            Turn tempTurn;
            tempTurn.push(turn.first(), turn.piece);
            tempTurn.push(Coords(x, turn.first().y));

            if (wouldPutInCheck(tempTurn, whoseTurn)) {
                return true;
            }
        }
    }
    else { //queenside castling
        for (int x = turn.first().x - 1; x >= turn.second().x; x--) {
            Turn tempTurn;
            tempTurn.push(turn.first(), turn.piece);
            tempTurn.push(Coords(x, turn.first().y));

            if (wouldPutInCheck(tempTurn, whoseTurn)) {
                return true;
            }
        }
    }

    return false;
}

bool Game::castlingBlocked(const Coords &rookStartLoc, const bool &kingSideCastle) {
    if (kingSideCastle) {
        for (int x = turn.first().x + 1; x < rookStartLoc.x; x++) {
            if (board.occupied(Coords(x, turn.first().y))) {
                return true;
            }
        }
    }
    else { //queenside castling
        for (int x = turn.first().x - 1; x > rookStartLoc.x; x--) {
            if (board.occupied(Coords(x, turn.first().y))) {
                return true;
            }
        }
    }

    return false;
}

bool Game::legalEnPassent() {
    if (turn.piece == Enums::wpawn) {
        if (turn.first().y == 5) {
            if (abs(turn.first().x - turn.second().x) == 1) {
                if (!board.occupied(turn.second()) && board.occupied(Coords(turn.second().x, turn.second().y-1))
                && board.getPiece(Coords(turn.second().x, turn.second().y-1)) == Enums::bpawn && startPawnMove(movesPlayed.back())) {
                    if (!wouldPutInCheck(turn, whoseTurn)) {
                        return true;
                    }
                }
            }
        }
    }
    if (turn.piece == Enums::bpawn) {
        if (turn.first().y == 4) {
            if (abs(turn.first().x - turn.second().x) == 1) {
                if (!board.occupied(turn.second()) && board.occupied(Coords(turn.second().x, turn.second().y+1))
                && board.getPiece(Coords(turn.second().x, turn.second().y+1)) == Enums::wpawn && startPawnMove(movesPlayed.back())) {
                    if (!wouldPutInCheck(turn, whoseTurn)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Game::startPawnMove(const Turn &givenTurn) {
    if (givenTurn.piece == Enums::wpawn) {
        if (givenTurn.first().y == 2 && givenTurn.second().y == 4
                && givenTurn.first().x == turn.second().x) {
            return true;
        }
    }
    if (givenTurn.piece == Enums::bpawn) {
        if (givenTurn.first().y == 7 && givenTurn.second().y == 5
                && givenTurn.first().x == turn.second().x) {
            return true;
        }
    }
    return false;
}

bool Game::insufficientMaterial() {
    if (board.board.size() == 1) {
        //some cataclysmic error occurred (spelling, whatevs)
        std::cout << "king removed from board somehow" <<std::endl;
        return true;
    }

    if (board.board.size() == 2) {
        return true; //just the 2 kings
    }
    if (board.board.size() == 3) {
        for (Board::boardConstIter iter = board.board.begin(); iter != board.board.end(); iter++) {
            if (iter->second == Enums::wbishop || iter->second == Enums::bbishop ||
                iter->second == Enums::wknight || iter->second == Enums::bknight) {
                //two kings + a knight or bishop
                return true;
            }
        }
    }
    if (board.board.size() == 4) {
        Board::boardConstIter wbish = board.board.end();
        Board::boardConstIter bbish = board.board.end();

        for (Board::boardConstIter iter = board.board.begin(); iter != board.board.end(); iter++) {
            if (iter->second == Enums::wbishop) {
                wbish = iter;
            }
            if (iter->second == Enums::bbishop) {
                bbish = iter;
            }
        }

        if (wbish != board.board.end() && bbish != board.board.end()) { //if only 2 kings + 2 bishops on board
                                                                        //(one for white one for black)
            if (sameColour(wbish->first, bbish->first)) {
                return true; //if the bishops on same colour, no checkmate possible
                //though, even if bishops not on same colour, is bascially draw anyway unless intentional throw
                //just implemented this case for completeness
            }
        }
    }
    return false;
}

bool Game::sameColour(const Coords &a, const Coords &b) {
    return getColour(a) == getColour(b);
}

bool Game::getColour(const Coords &coords) {
    //true = light
    //false = dark

    //noticed while coding that all light squares have an odd total in their coords
    //and all dark squares an even total
    //e.g a1 = 1,1 = 2 = black
    //    d4 = 4,4 = 8 = black
    //    h3 = 8,3 = 11 = white

    if (coords.x + coords.y % 2) {
        return true;
    }
    //else
    return false;
}

bool Game::inactivity() {
    if (movesPlayed.size() >= 100) {
        Board tempBoard; //temporary board to play through previous moves
        tempBoard.setup(); //used to check for any captures
        for (std::vector<Turn>::const_iterator iter = movesPlayed.begin(); iter < movesPlayed.end() - 100; iter++) {
            //catching up to the last 50 moves
            tempBoard.movePiece(*iter);
        }

        //now to check fro captures/pawnmoves in last 50 moves
        for (std::vector<Turn>::const_iterator iter = movesPlayed.end() - 100; iter < movesPlayed.end(); iter++) {
            if (iter->piece == Enums::wpawn || iter->piece == Enums::bpawn) {
                //pawn was moved on this turn
                return false;
            }
            if (tempBoard.occupied(iter->second())) {
                //piece was captured on this turn
                return false;
            }

            tempBoard.movePiece(*iter);
        }
        //no pawn moves or captures
        return true;
    }

    return false;
}

bool Game::repetition() {
    if (movesPlayed.size() >= 6) {
        Board tempBoard;
        tempBoard.setup();

        int repCount = 0; //will count the last move as well, and current position
        //counts as one of the repetitions

        for (std::vector<Turn>::const_iterator iter = movesPlayed.begin(); iter < movesPlayed.end(); iter++) {
            tempBoard.movePiece(*iter);

            if (board.board == tempBoard.board) {
                ++repCount;
            }
        }

        if (repCount >= 3) {
            return true;
        }
    }
    return false;
}

void Game::saveGame() {
    std::ofstream fileOut("savedgames/game.txt");

    for (std::vector<Turn>::const_iterator iter = movesPlayed.begin(); iter < movesPlayed.end(); iter++) {
        fileOut << "\n" << *iter;
    }

    fileOut.close();
}

void Game::loadGame() {
    movesPlayed.clear();
    movesShown.clear();

    std::ifstream fileIn("savedgames/game.txt");

    while (fileIn.good()) {
        Turn tempTurn;
        fileIn >> tempTurn;
        movesPlayed.push_back(tempTurn);
    }

    fileIn.close();
}


void Game::startNewGame() {
    progState = Enums::liveGame;

    gameState = Enums::live;
    whoseTurn = true;
    drawOffered = false;
    movesPlayed.clear();
    board = Board();
    board.setup();
}

void Game::loadOldGame() {
    progState = Enums::loadedGame;

    whoseTurn = true;
    movesPlayed.clear();
    board = Board();
    board.setup();
    loadGame();
}

void Game::loadPrevMove() {
    Turn reverseTurn;
    reverseTurn.push(movesShown.back().first.second(), movesShown.back().first.piece);
    reverseTurn.push(movesShown.back().first.first()); //i know this is terrible, i know
    reverseTurn.moveType = movesShown.back().first.moveType;
    board.movePiece(reverseTurn);

    if (movesShown.back().second < 12) {  //0-11 are the int equivalents of the actual piece enums, iknow
        //will be here if piece was taken last turn
        std::pair<Coords, Enums::PieceID> takenPiece = std::make_pair(movesShown.back().first.second(), movesShown.back().second);
        board.board.insert(takenPiece);
    }
    //if castle, the rook move would have just been performed and now the king move is next
    else if (movesShown.back().first.moveType == Enums::castle) {
        movesShown.pop_back();

        Turn reverseKingTurn;
        reverseKingTurn.push(movesShown.back().first.second(), movesShown.back().first.piece);
        reverseKingTurn.push(movesShown.back().first.first()); //i know this is terrible, i know
        reverseKingTurn.moveType = movesShown.back().first.moveType;
        board.movePiece(reverseKingTurn);
    }
    else if (moveTypePromotion(reverseTurn.moveType)) {
        board.board[reverseTurn.second()] = whoseTurn ? Enums::bpawn : Enums::wpawn;
    }

    movesShown.pop_back();
    whoseTurn = (whoseTurn ? false : true); //reverse whoseTurn
}

void Game::loadNextMove() {
    Turn nextTurn = movesPlayed[movesShown.size()];

    Enums::PieceID takenPiece = Enums::board; //anything 'greater than' the actual pieces
        //I KNOW THIS IS ABSOLUTELY HORRIBLE CODE, DEAL WITH IT
    if (board.occupied(nextTurn.second())) {
         takenPiece = board.getPiece(nextTurn.second());
    }

    board.movePiece(nextTurn);
    movesShown.push_back(std::make_pair(nextTurn, takenPiece));

    if (nextTurn.moveType == Enums::castle) {
        Turn rookTurn = movesPlayed[movesShown.size()];
        board.movePiece(rookTurn);
        movesShown.push_back(std::make_pair(rookTurn, takenPiece)); //if castled, a piece cannot have been taken
        //taken piece will still be Enums::board
        //loadNextMove();
    }
    else if (moveTypePromotion(nextTurn.moveType)) {
        switch (nextTurn.moveType) {
            case Enums::promoteBishop: {
                board.board[nextTurn.second()] = whoseTurn ? Enums::wbishop : Enums::bbishop;
                break;
            }
            case Enums::promoteKnight: {
                board.board[nextTurn.second()] = whoseTurn ? Enums::wknight : Enums::bknight;
                break;
            }
            case Enums::promoteRook: {
                board.board[nextTurn.second()] = whoseTurn ? Enums::wrook : Enums::brook;
                break;
            }
            case Enums::promoteQueen: {
                board.board[nextTurn.second()] = whoseTurn ? Enums::wqueen : Enums::bqueen;
                break;
            }
            default: {
                //error
                std::cout << "moveTypePromotion failed" <<std::endl;
            }
        }
    }

    whoseTurn = (whoseTurn ? false : true); //reverse whoseTurn
}

bool Game::moveTypePromotion(const Enums::TurnType &turnType) {
    switch (turnType) {
        case Enums::promoteBishop:
        case Enums::promoteKnight:
        case Enums::promoteRook:
        case Enums::promoteQueen: {
            return true;
        }
        default: {
            return false;
        }
    }
}

void Game::checkGameOver() {
    if (inCheck(!whoseTurn)) { //if opponent has been put in check
        //see if in checkmate
        if (noLegalMoves(!whoseTurn)) {
            if (whoseTurn) {
                gameState = Enums::whiteWinMate;
            }
            else {
                gameState = Enums::blackWinMate;
            }
        }
    }
    else { //not in check
        //see if in stalemate
        if (noLegalMoves(!whoseTurn)) {
            gameState = Enums::drawStalemate;
        }
    }

    if (board.board.size() < 5) { //if potentially insufficient material
        if (insufficientMaterial()) {
            gameState = Enums::drawInsufficient;
        }
    }
}

void Game::makeMove() {
    board.movePiece(turn);

    Turn rookMove; //perhaps used in if statement below,
    bool castled = false; //needs this scope level though
    if (castlingRequested()) { //at this point, castling has effectively been done, unfortunate function naming
        //king moved to it's correct spot, must also move rook, see comments at movesPlayed declaration in Game.h
        turn.moveType = Enums::castle;

        Enums::PieceID rook = (whoseTurn ? Enums::wrook : Enums::brook);
        Coords rookStartLoc((turn.first().x < turn.second().x ? 8 : 1), (whoseTurn ? 1 : 8));

        rookMove.push(rookStartLoc, rook);
        rookMove.push(Coords(rookStartLoc.x == 1 ? 4 : 6, rookStartLoc.y));
        rookMove.moveType = Enums::castle;
        castled = true;

        board.movePiece(rookMove);
    }

    movesPlayed.push_back(turn);
    if (castled) {
        movesPlayed.push_back(rookMove);
    }
}

void Game::processTurn() {
    if (promotionNeeded()) {
        gameState = Enums::promotion;
        return; //want to promote the piece before doing anything else
    } //once piece is promoted (e.g, pawn on a7 (before being moved) has become a queen on a7
    //then move the piece to a8

    makeMove();
    checkGameOver();

    turn.reset(); //reset the turn member
    whoseTurn = (whoseTurn ? false : true); //reverse whoseTurn
}
