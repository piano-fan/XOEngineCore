#include "piecemask.h"
#include <cassert>


namespace XO{
    constexpr PieceMask8::MaskID EmptyMask(){
        PieceMask8::MaskID result = 0;
        for(int i = 0; i < 8; i++){
            PieceMask8::MaskID empty = EMPTY << (i << 1);
            result |= empty;
        }
        return result;
    }

    PieceMask8::MaskID PieceMask8::operator++(){
        m_mask++;
        return m_mask - EmptyMask();
    }

    PieceMask8::PieceMask8():
        m_mask(EmptyMask())
    {}

    PieceMask8::MaskID PieceMask8::GetID() const{
        return m_mask;
    }

    Piece PieceMask8::GetPiece(int i) const{
        assert(i >= 0);
        assert(i < 8);
        return static_cast<Piece>((m_mask >> (i << 1)) & 3);
    }

    void PieceMask8::ClearPiece(int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == X || GetPiece(i) == O);
        m_mask &= ~(3 << (i << 1));
        m_mask |= static_cast<MaskID>(EMPTY) << (i << 1);
    }

    void PieceMask8::SetPiece(Piece p, int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == EMPTY);
        m_mask &= ~(3 << (i << 1));
        m_mask |= static_cast<MaskID>(p) << (i << 1);
    }

    bool PieceMask8::IsWin(Piece p) const{
        Piece current = OUT_OF_BOUNDS;
        uint8_t count = 0;
        for(int i = 0; i < 8; i++){
            Piece next = GetPiece(i);
            if(next != current){
                count = 1;
                current = next;
            }
            else{
                count++;
                if(current == p && count >= 4){
                    return true;
                }
            }
        }
        return false;
    }

    std::bitset<8> PieceMask8::TestNullMoves(Piece piece, std::function<bool(PieceMask8, Piece)> &P) const{
        std::bitset<8> result(0);
        for(int i = 0; i < 8; i++){
            Piece next = GetPiece(i);
            if(next == EMPTY){
                PieceMask8 test = *this;
                test.SetPiece(piece, i);
                if(P(test, piece)) {
                    result.set(i);
                }
            }
        }
        return result;
    }

    std::string PieceMask8::ToString() const{
        std::string result = "Mask ID: " + std::to_string(GetID()) + " ";
        for(int i = 0; i < 8; i++){
            result += PieceName[GetPiece(i)];
        }
        return result;
    }
}