#include <cassert>
#include "piece.h"

namespace XO {
    Piece OppositePiece(Piece p) {
        if (p == X) {
            return O;
        }
        assert (p == O);
        return X;
    }
}