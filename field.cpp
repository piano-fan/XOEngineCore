#include <cassert>
#include "field.h"


namespace XO {
    int Field::GetSquareID(int x, int y) {
        assert(InBounds(x, y));
        return x + y * m_width;
    }

    Field::Field(unsigned int w, unsigned int h) :
            m_width(w), m_height(h), m_square_count(w*h), m_squares(w * h, this) {
        for(int x = 0; x < m_width; x++){
            for(int y = 0; y < m_height; y++){
                m_squares[GetSquareID(x, y)].Init(x, y);
            }
        }
    }

    bool Field::InBounds(int id) const {
        return id >= 0 && id < m_square_count;
    }

    bool Field::InBounds(int x, int y) const{
        return x >= 0
               && y >= 0
               && x < m_width
               && y < m_height;
    }

    Square &Field::GetSquare(int id) {
        assert(InBounds(id));
        return m_squares[id];
    }

    Square& Field::GetSquare(int x, int y) {
        return m_squares[GetSquareID(x, y)];
    }

    unsigned int Field::GetWidth() const{
        return m_width;
    }

    unsigned int Field::GetHeight() const{
        return m_height;
    }

    unsigned int Field::GetSquareCount() const{
        return m_square_count;
    }

    FieldIterator Field::Middle() {
        return GetSquare(m_width / 2, m_height / 2).Iter();
    }
}