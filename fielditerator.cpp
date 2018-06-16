#include "field.h"
#include "direction.h"
#include <cassert>


namespace XO{
    FieldIterator::FieldIterator(Field* f, int x, int y):
            m_field(f), m_x(x), m_y(y)
    {
        if(f) {
            m_in_bounds = m_field->InBounds(x, y);
        }
    }

    const Square& FieldIterator::operator*() const{
        assert(m_field);
        assert(m_in_bounds);
        return m_field->GetSquare(m_x, m_y);
    }

    Piece FieldIterator::GetPiece() const{
        assert(m_field);
        if(m_in_bounds){
            return m_field->GetSquare(m_x, m_y).GetPiece();
        }
        else{
            return OUT_OF_BOUNDS;
        }
    }

    FieldIterator& FieldIterator::operator++(){
        assert(m_field);
        assert(m_in_bounds);
        m_x++;
        if (m_x >= m_field->GetWidth()) {
            m_x = 0;
            m_y++;
            if(m_y >= m_field->GetHeight()){
                m_in_bounds = false;
            }
        }
        return *this;
    }

    FieldIterator& FieldIterator::SeekEmpty(){
        assert(m_field);
        assert(m_in_bounds);
        Piece p = GetPiece();
        while(p != OUT_OF_BOUNDS && p != EMPTY){
            p = (++*this).GetPiece();
        }
        return *this;
    }

    FieldIterator& FieldIterator::Step(int dir, bool mirror){
        assert(m_field);
        if(mirror){
            m_x -= Direction::GetX(dir);
            m_y -= Direction::GetY(dir);
        }
        else{
            m_x += Direction::GetX(dir);
            m_y += Direction::GetY(dir);
        }
        m_in_bounds = m_field->InBounds(m_x, m_y);
        return *this;
    }

    int FieldIterator::GetX() const{
        return m_x;
    }

    int FieldIterator::GetY() const{
        return m_y;
    }

    int FieldIterator::GetValue(Piece p) const{
        return m_field->GetSquare(m_x, m_y).GetValue(p);
    }

    PieceMask8 FieldIterator::MakePiecemask8(int dir) const{
        auto result = PieceMask8();
        FieldIterator first = *this;
        FieldIterator second = first;

        for(int i = 3, j = 4; j < 8; i--, j++){
            result.SetPiece(first.Step(dir, true).GetPiece(), i);
            result.SetPiece(second.Step(dir, false).GetPiece(), j);
        }
        return result;
    }
}