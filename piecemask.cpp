#include "piecemask.h"
#include <cassert>


namespace XO{
    PieceMask8::MaskID PieceMask8::operator++(){
        m_mask++;
        return m_mask;
    }

    PieceMask8::PieceMask8():
        m_mask(0)
    {}

    PieceMask8::MaskID PieceMask8::GetID(){
        return m_mask;
    }

    Piece PieceMask8::GetPiece(int i){
        assert(i >= 0);
        assert(i < 8);
        return static_cast<Piece>((m_mask >> (i << 1)) & 3);
    }

    void PieceMask8::ClearPiece(int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == X || GetPiece(i) == O);
        m_mask &= ~(3 << (i << 1));
    }

    void PieceMask8::SetPiece(Piece p, int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == EMPTY);
        m_mask |= static_cast<MaskID>(p) << (i << 1);
    }

    std::string PieceMask8::ToString(){
        std::string result = "Mask ID: " + std::to_string(GetID()) + " ";
        for(int i = 0; i < 8; i++){
            result += PieceName[GetPiece(i)];
        }
        return result;
    }
}