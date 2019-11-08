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

    class PieceSet32{
    public:
        using ID = uint64_t;
        static constexpr int CAPACITY = 4 * sizeof(ID);
        static_assert(CAPACITY == 32);

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

    public:
        constexpr PieceSet32()
                :m_data(Empty())
        {}

        constexpr ID operator++(){
            m_data++;
            return m_data - Empty();
        }

        constexpr ID GetID() const{
            return m_data;
        }

        constexpr Piece GetPiece(int i) const{
            return static_cast<Piece>((m_data >> (i << 1)) & 3);
        }
        constexpr void SetPiece(Piece p, int i){
            m_data &= ~(static_cast<ID>(3) << (i << 1));
            m_data |= static_cast<ID>(p) << (i << 1);
        }

        std::string ToString() const{
            std::string result = "Mask ID: " + std::to_string(GetID()) + " ";
            for(int i = 0; i < CAPACITY; i++) {
                result += PieceNameShort[GetPiece(i)];
            }
            return result;
        }
    };

    class DynamicPieceSet{
        std::vector<PieceSet32> m_data;
    public:
        void Resize(DValueT count){
            m_data.resize((count - 1) / 32 + 1, PieceSet32());
        }

        void Clear(){
            m_data.assign(m_data.size(), PieceSet32());
        }

        Piece GetPiece(Point t) const{
            return m_data[t.GetID() / 32].GetPiece(t.GetID() % 32);
        }

        void SetPiece(Piece p, Point t){
            m_data[t.GetID() / 32].SetPiece(p, t.GetID() % 32);
        }

        void SetPiece(const Move& m){
            SetPiece(m.GetTurn(), m.GetPos());
        }

        void RemovePiece(const Move& m){
            SetPiece(EMPTY, m.GetPos());
        }

        bool operator<(const DynamicPieceSet& other) const{
            assert(m_data.size() == other.m_data.size());
            for(int i = 0; i < m_data.size(); ++i){
                if(m_data[i].GetID() < other.m_data[i].GetID()){
                    return true;
                }
                if(m_data[i].GetID() > other.m_data[i].GetID()){
                    break;
                }
            }
            return false;
        }

        std::string ToString() const{
            auto result = std::string();
            for(auto& pset: m_data){
                result += pset.ToString() + "\n";
            }
            return result;
        }
    };
}


#endif //XO_PIECESET_H
