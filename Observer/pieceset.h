#ifndef XO_PIECESET_H
#define XO_PIECESET_H

#include "types.h"


namespace XO{
    class PieceSet8{
    public:
        using ID = uint16_t;
        static constexpr int TOTAL_COUNT = 65536;
        static constexpr int CAPACITY = 4 * sizeof(ID);
        static_assert(CAPACITY == 8);

    private:
        ID m_data;

        static constexpr ID Empty(){
            ID result = 0;
            for(int i = 0; i < CAPACITY; i++){
                ID empty = static_cast<ID>(EMPTY) << (i << 1);
                result |= empty;
            }
            return result;
        }

        constexpr ValueT Offset1DtoBitID(const Offset1D& offset) const{
            return (offset.Distance() << 1) + (offset.Direction() << 3);
        }

    public:
        constexpr PieceSet8()
            :m_data(Empty())
        {}

        constexpr ID operator++(){
            m_data++;
            return m_data - Empty();
        }

        constexpr ID GetID() const{
            return m_data;
        }

        constexpr Piece GetPiece(Offset1D offset) const{
            return static_cast<Piece>((m_data >> Offset1DtoBitID(offset)) & 3);
        }
        constexpr void SetPiece(Piece p, Offset1D offset){
            m_data &= ~(static_cast<ID>(3) << Offset1DtoBitID(offset));
            m_data |= static_cast<ID>(p) << Offset1DtoBitID(offset);
        }

        constexpr bool IsWin(Piece p) const{
            uint8_t count = 0;
            for(int dir = 0; dir < 2; ++dir) {
                for (int i = 0; i < 4; i++) {
                    Piece next = GetPiece(Offset1D(dir, i));
                    if (next == p) {
                        count++;
                        if(count >= 4){
                            return true;
                        }
                    } else {
                        break;
                    }
                }
            }
            return false;
        }

        constexpr ValueT SolidBlockLength(Piece p) const{
            ValueT counter = 0;
            for(auto dir = 0; dir < 2; ++dir){
                for(auto id = 0; id < 4; ++id){
                    if(GetPiece(Offset1D(dir, id)) != p){
                        break;
                    }
                    ++counter;
                }
            }
            return counter;
        }

        std::string ToString() const{
            std::string result = "Mask ID: " + std::to_string(GetID()) + " ";
            for(int dir = 0; dir < 2; ++dir) {
                result += ">";
                for (int id = 0; id < 4; id++) {
                    result += PieceNameShort[GetPiece(Offset1D(dir, id))];
                }
            }
            return result;
        }
    };
}


#endif //XO_PIECESET_H
