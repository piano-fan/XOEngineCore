#ifndef XO_PIECE_H
#define XO_PIECE_H

#include <string>


namespace XO {
    const std::string PieceName[] = {"~", "X", "O", "#"};

    enum Piece {
        EMPTY = 0,
        X = 1,
        O = 2,
        OUT_OF_BOUNDS = 3
    };

    enum PieceID {
        XID = 0,
        OID = 1,
    };

    PieceID GetPieceID(Piece p);
    Piece OppositePiece(Piece p);
}

#endif //XO_PIECE_H
