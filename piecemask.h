#ifndef XO_PIECEMASK_H
#define XO_PIECEMASK_H

#include <cstdint>
#include <string>
#include "piece.h"


namespace XO{
    class PieceMask8{
    public:
        using MaskID = uint16_t;

    private:
        MaskID m_mask;

    public:
        PieceMask8();
        MaskID operator++();

        MaskID GetID();
        Piece GetPiece(int i);
        void ClearPiece(int i);
        void SetPiece(Piece p, int i);

        std::string ToString();
    };
}


#endif //XO_PIECEMASK_H
