#ifndef XO_PIECE_H
#define XO_PIECE_H

#include "builtintypes.h"


namespace XO {
    const std::string PieceName[] = {"Ally", "Enemy", "Empty", "OutOfBounds"};
    const std::string PieceNameShort[] = {"A", "E", "~", "#"};

    enum Piece {
        ALLY = 0,
        ENEMY = 1,
        EMPTY = 2,
        OUT_OF_BOUNDS = 3
    };
    
    constexpr const auto Players = {ALLY, ENEMY};

    inline Piece OppositePiece(Piece p){
        if (p == ALLY) {
            return ENEMY;
        }
        return ALLY;
    }
}

#endif //XO_PIECE_H
