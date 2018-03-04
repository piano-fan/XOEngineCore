#include <cassert>
#include "piece.h"

namespace XO {
    PieceID GetPieceID(Piece p){
        if(p == X){
            return XID;
        }
        else if (p == O){
            return OID;
        }
        assert(false);
    }

    Piece OppositePiece(Piece p) {
        if (p == X) {
            return O;
        } else if (p == O) {
            return X;
        }
        assert (false);
    }
}