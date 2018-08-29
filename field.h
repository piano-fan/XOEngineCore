#ifndef XO_FIELD_H
#define XO_FIELD_H

#include <vector>
#include <functional>
#include <cstdint>
#include "piecemask.h"

namespace XO {
    class Field {
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        std::vector <Piece> m_squares;

        int GetSquareID(int x, int y);
    public:
        Field(unsigned int w, unsigned int h);
        void Reset(unsigned int w, unsigned int h);

        Piece GetPiece(int x, int y);
        Piece PlacePiece(Piece p, int x, int y);
        Piece ClearPiece(int x, int y);

        int GetWidth();
        int GetHeight();
        int GetSquareCount();

        bool Middle(int &r_x, int &r_y);
    };

}

#endif //XO_FIELD_H
