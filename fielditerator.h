#ifndef XO_FIELDITERATOR_H
#define XO_FIELDITERATOR_H

#include "field.h"


namespace XO {
    class FieldIterator {
        Field *m_field;
        int m_x;
        int m_y;
        bool InBounds() const;
    public:
        explicit FieldIterator(Field *f, int x = 0, int y = 0);
        Piece operator++();
        Piece SeekEmpty();
        Piece Step(int dir, bool mirror);
        int GetX() const;
        int GetY() const;
        Piece GetPiece() const;
        bool IsWin(Piece p) const;
        PieceMask8 MakePiecemask8(int dir) const;
    };
}

#endif //XO_FIELDITERATOR_H
