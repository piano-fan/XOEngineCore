#include <cassert>
#include <iostream>
#include "field.h"

namespace XO {
    int Field::GetSquareID(int x, int y){
        assert(x >= 0);
        assert(x < m_width);
        assert(y >= 0);
        assert(y < m_height);
        return x + y * m_width;
    }

    Field::Field(unsigned int w, unsigned int h) :
            m_width(w), m_height(h), m_squares(w * h, EMPTY) {}

    void Field::Reset(unsigned int w, unsigned int h) {
        m_width = w;
        m_height = h;
        m_squares.assign(w * h, EMPTY);
    }

    Piece Field::GetPiece(int x, int y) {
        return m_squares[GetSquareID(x, y)];
    }

    Piece Field::PlacePiece(Piece p, int x, int y) {
        int id = GetSquareID(x, y);
        Piece current = m_squares[id];
        assert(current == EMPTY);
        m_squares[id] = p;
        return current;
    }

    Piece Field::ClearPiece(int x, int y) {
        int id = GetSquareID(x, y);
        Piece current = m_squares[id];
        assert(current == X || current == O);
        m_squares[id] = EMPTY;
        return current;
    }

    int Field::GetWidth() {
        return m_width;
    }

    int Field::GetHeight() {
        return m_height;
    }

    int Field::GetSquareCount() {
        return m_squares.size();
    }

    void Field::Print() {
        int x = 0;
        for (Piece &p: m_squares) {
            if (p == X) std::cout << "X";
            if (p == O) std::cout << "O";
            if (p == EMPTY) std::cout << "-";

            x++;
            if (x >= m_width) {
                x = 0;
                std::cout << std::endl;
            }
        }
    }

    bool Field::Middle(int &r_x, int &r_y) {
        r_x = m_width / 2;
        r_y = m_height / 2;
        return m_width && m_height;
    }
}