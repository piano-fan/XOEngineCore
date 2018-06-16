#ifndef XO_PIECEMASK_H
#define XO_PIECEMASK_H

#include <functional>
#include <bitset>
#include "piece.h"
#include "types.h"


namespace XO{
    class PieceMask8{
    public:
        using MaskID = uint16_t;

    private:
        MaskID m_mask;

    public:
        PieceMask8();
        MaskID operator++();

        MaskID GetID() const;
        Piece GetPiece(int i) const;
        void ClearPiece(int i);
        void SetPiece(Piece p, int i);

        bool IsWin(Piece p) const;
        std::bitset<8> TestNullMoves(Piece piece, std::function<bool(PieceMask8, Piece)> &P) const;

        std::string ToString() const;
    };
}


#endif //XO_PIECEMASK_H
