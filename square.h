#ifndef XO_SQUARE_H
#define XO_SQUARE_H

#include <array>
#include <string>
#include "piecemask.h"
#include "threatinfo.h"
#include "fielditerator.h"


namespace XO {
    const ValueT FINISHER_SCORE = 31;   //TODO: remove/refactor

    class Field;

    struct SquareScore{
        int m_value[2];
        ValueT m_modified = 1;
    };

    class Square {
        Field* m_field;
        int m_x;
        int m_y;
        Piece m_piece;

        mutable std::array<PieceMask8, 4> m_mask_cache;
        mutable SquareScore m_sqinfo;

        void UpdateSqInfo() const;
    public:
        Square(Field* parent);
        void Init(int x, int y);

        Piece GetPiece() const;
        Piece PlacePiece(Piece p);
        Piece ClearPiece();

        PieceMask8 GetPieceMask(int dir) const;
        bool IsWin(Piece p) const;
        int GetValue(Piece p) const;

        FieldIterator Iter() const;

        std::string ToString() const;
    };
}

#endif //XO_SQUARE_H
