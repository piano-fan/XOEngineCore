#include "pieceset.h"
#include <cassert>


namespace XO{
    constexpr PieceSet8::PieceSetID EmptyMask(){
        PieceSet8::PieceSetID result = 0;
        for(int i = 0; i < 8; i++){
            PieceSet8::PieceSetID empty = EMPTY << (i << 1);
            result |= empty;
        }
        return result;
    }

    PieceSet8::PieceSetID PieceSet8::operator++(){
        m_bits++;
        return m_bits - EmptyMask();
    }

    PieceSet8::PieceSet8():
        m_bits(EmptyMask())
    {}

    Piece PieceSet8::GetPiece(int i) const{
        assert(i >= 0);
        assert(i < 8);
        return static_cast<Piece>((m_bits >> (i << 1)) & 3);
    }

    void PieceSet8::ClearPiece(int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == X || GetPiece(i) == O);
        m_bits &= ~(3 << (i << 1));
        m_bits |= static_cast<PieceSetID>(EMPTY) << (i << 1);
    }

    void PieceSet8::SetPiece(Piece p, int i){
        assert(i >= 0);
        assert(i < 8);
        assert(GetPiece(i) == EMPTY);
        m_bits &= ~(3 << (i << 1));
        m_bits |= static_cast<PieceSetID>(p) << (i << 1);
    }

    bool PieceSet8::IsWin(Piece p) const{
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

    std::bitset<8> PieceSet8::TestNullMoves(Piece piece, std::function<bool(PieceSet8, Piece)> &P) const{
        std::bitset<8> result(0);
        for(int i = 0; i < 8; i++){
            Piece next = GetPiece(i);
            if(next == EMPTY){
                PieceSet8 test = *this;
                test.SetPiece(piece, i);
                if(P(test, piece)) {
                    result.set(i);
                }
            }
        }
        return result;
    }

    std::string PieceSet8::ToString() const{
        std::string result = "Mask ID: " + std::to_string(GetID()) + " ";
        for(int i = 0; i < 8; i++){
            result += PieceName[GetPiece(i)];
        }
        return result;
    }
}
