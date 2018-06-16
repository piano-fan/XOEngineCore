#ifndef XO_FIELD_H
#define XO_FIELD_H

#include <vector>
#include <array>
#include <memory>
#include "fielditerator.h"
#include "square.h"


namespace XO {
    class Field {
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        unsigned int m_square_count = 0;
        std::vector <Square> m_squares;

        int GetSquareID(int x, int y);
    public:
        Field(unsigned int w, unsigned int h);

        bool InBounds(int id) const;
        bool InBounds(int x, int y) const;
        Square& GetSquare(int id);
        Square& GetSquare(int x, int y);

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        unsigned int GetSquareCount() const;

        FieldIterator Middle() ;
    };
}

#endif //XO_FIELD_H
