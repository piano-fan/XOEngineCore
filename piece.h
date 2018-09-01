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

    inline Piece OppositePiece(Piece p){
        if (p == X) {
            return O;
        }
        return X;
    }
}

#endif //XO_PIECE_H
