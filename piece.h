#ifndef XO_PIECE_H
#define XO_PIECE_H

#include <string>


namespace XO {
    const std::string PieceName[] = {"X", "O", "~", "#"};

    enum Piece {
        X = 0,
        O = 1,
        EMPTY = 2,
        OUT_OF_BOUNDS = 3
    };

    Piece OppositePiece(Piece p);
}

#endif //XO_PIECE_H
