#include "fielditerator.h"
#include "direction.h"
#include <cassert>


namespace XO{
    bool FieldIterator::InBounds() const{
        return m_x >= 0
            && m_y >= 0
            && m_x < m_field->GetWidth()
            && m_y < m_field->GetHeight();
    }

    FieldIterator::FieldIterator(Field* f, int x, int y):
            m_field(f), m_x(x), m_y(y)
    {
        assert(InBounds());
    }

    Piece FieldIterator::operator++(){
        assert(InBounds());
        m_x++;
        if (m_x >= m_field->GetWidth()) {
            m_x = 0;
            m_y++;
            if (m_y >= m_field->GetHeight()) {
                return OUT_OF_BOUNDS;
            }
        }
        return m_field->GetPiece(m_x, m_y);
    }

    Piece FieldIterator::SeekEmpty(){
        Piece p = GetPiece();
        while(p != OUT_OF_BOUNDS && p != EMPTY){
            p = ++*this;
        }
        return p;
    }

    Piece FieldIterator::Step(int dir, bool mirror){
        if(mirror){
            m_x -= Direction::GetX(dir);
            m_y -= Direction::GetY(dir);
        }
        else{
            m_x += Direction::GetX(dir);
            m_y += Direction::GetY(dir);
        }
        return GetPiece();
    }

    int FieldIterator::GetX() const{
        return m_x;
    }

    int FieldIterator::GetY() const{
        return m_y;
    }

    Piece FieldIterator::GetPiece() const{
        if(InBounds()){
            return m_field->GetPiece(m_x, m_y);
        }
        else{
            return OUT_OF_BOUNDS;
        }
    }

    bool FieldIterator::IsWin(Piece p) const{
        for(int dir = 0; dir < 4; dir ++){
            int count = 0;
            FieldIterator first = *this;
            FieldIterator second = first;
            while(first.Step(dir, true) == p){
                count++;
            }
            while(second.Step(dir, false) == p){
                count++;
            }
            if(count >= 4){
                return true;
            }
        }
        return false;
    }

    PieceMask8 FieldIterator::MakePiecemask8(int dir) const{
        auto result = PieceMask8();
        FieldIterator first = *this;
        FieldIterator second = first;

        for(int i = 3, j = 4; j < 8; i--, j++){
            result.SetPiece(first.Step(dir, true), i);
            result.SetPiece(second.Step(dir, false), j);
        }
        return result;
    }
}