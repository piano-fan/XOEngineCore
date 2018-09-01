#ifndef XO_PIECESET_H
#define XO_PIECESET_H

#include <functional>
#include <bitset>
#include "types.h"


namespace XO{
    const int TOTAL_MASK_COUNT = 65536;

    class PieceSet8{
    public:
        using PieceSetID = uint16_t;

    private:
        PieceSetID m_bits;

    public:
        PieceSet8();
        PieceSetID operator++();

        PieceSetID GetID() const{
            return m_bits;
        }

        Piece GetPiece(int i) const;
        void ClearPiece(int i);
        void SetPiece(Piece p, int i);

        bool IsWin(Piece p) const;
        std::bitset<8> TestNullMoves(Piece piece, std::function<bool(PieceSet8, Piece)> &P) const;

        std::string ToString() const;
    };
}


#endif //XO_PIECESET_H
