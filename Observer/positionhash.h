#ifndef DYNAMIC_PIECESET_H
#define DYNAMIC_PIECESET_H

#include "types.h"


namespace XO{
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

    class PositionHash{
        std::vector<PieceSet32> m_data;

        int PSet_ID(DOffsetT sq_id) const { return sq_id >> 5; }        //sq_id / 32
        int PSetItem_ID(DOffsetT sq_id) const { return sq_id & 0x1F; }  //sq_id % 32

    public:
        void Resize(DValueT sq_count){
            m_data.resize((sq_count - 1) / 32 + 1, PieceSet32());
        }

        void Clear(){
            m_data.assign(m_data.size(), PieceSet32());
        }

        Piece GetPiece(DOffsetT sq_id) const{
            return m_data[PSet_ID(sq_id)].GetPiece(PSetItem_ID(sq_id));
        }

        void SetPiece(Piece p, DOffsetT sq_id){
            m_data[PSet_ID(sq_id)].SetPiece(p, PSetItem_ID(sq_id));
        }

        void SetPiece(const Move& m){
            SetPiece(m.GetTurn(), m.GetPos().GetID());
        }

        void RemovePiece(const Move& m){
            SetPiece(EMPTY, m.GetPos().GetID());
        }

        bool operator<(const PositionHash& other) const{
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

#endif //DYNAMIC_PIECESET_H
