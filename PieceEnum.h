#ifndef PIECEENUM_H_INCLUDED
#define PIECEENUM_H_INCLUDED

namespace Enums {
    enum PieceID {
        wpawn = 0,
        wbishop,
        wrook,
        wknight,
        wqueen,
        wking,
        bpawn,
        bbishop,
        brook,
        bknight,
        bqueen,
        bking,
        board,
        highlightBorder,
        highlightFill,
        promoteWhite,
        promoteBlack,
        resignButton,
        drawButton,
        messageBox,
        button
    };

    enum TurnState {
        clearTurn = 0,
        sourceDeclared,
        targetDeclared
    };

    enum GameState {
        live = 0,
        dead,
        promotion,
        drawAgreed,
        drawStalemate,
        drawRepetition,
        drawInactivity,
        drawInsufficient,
        whiteWinMate,
        whiteWinResign,
        blackWinMate,
        blackWinResign
    };

    enum TurnType {
        normal = 0,
        castle,
        enPassent,
        promoteBishop,
        promoteKnight,
        promoteRook,
        promoteQueen,
    };

    enum OverallState {
        liveGame = 0,
        loadedGame,
        startup
    };
}

#endif // PIECEENUM_H_INCLUDED
