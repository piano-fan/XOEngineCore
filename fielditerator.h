#ifndef XO_FIELDITERATOR_H
#define XO_FIELDITERATOR_H

#include "piecemask.h"


namespace XO {
    class Field;
    class Square;

    class FieldIterator {
        Field *m_field;
        int m_x;
        int m_y;
        bool m_in_bounds;
    public:
        explicit FieldIterator(Field *f = nullptr, int x = 0, int y = 0);
        const Square& operator*() const;
        Piece GetPiece() const;
        FieldIterator& operator++();
        FieldIterator& SeekEmpty();
        FieldIterator& Step(int dir, bool mirror);
        int GetX() const;
        int GetY() const;
        int GetValue(Piece p) const;
        PieceMask8 MakePiecemask8(int dir) const;
    };
}

#endif //XO_FIELDITERATOR_H
