#ifndef XO_SQUAREINFLUENCE_H
#define XO_SQUAREINFLUENCE_H

#include "types.h"
#include "pieceset.h"
#include "sortedthreatset.h"


namespace XO{
    class SquareInfluence{
        std::array<PieceSet8, 4> m_psets;
        std::array<SortedThreatSet, 2> m_threats;

        void UpdateThreats(ValueT dir);
    public:
        void ClearPSets(){
            m_psets.fill(PieceSet8());
        }
        void AddBounds(StarOffset where){
            m_psets[where.Angle()].SetPiece(OUT_OF_BOUNDS, where.Offset());
        }

        void Calculate();
        void OnNeighbourChanged(Piece p, StarOffset self);

        const PieceSet8& GetPieceSet(ValueT dir) const{
            return m_psets[dir];
        }
        const SortedThreatSet& GetThreats(Piece p) const{
            return m_threats[p];
        }

        std::string ToString() const{
            return GetPieceSet(0).ToString() + "\n"
                   + GetPieceSet(1).ToString() + "\n"
                   + GetPieceSet(2).ToString() + "\n"
                   + GetPieceSet(3).ToString() + "\n"
                   + PieceName[ALLY] + ": " + GetThreats(ALLY).ToString()
                   + PieceName[ENEMY] + ": " + GetThreats(ENEMY).ToString();
        }
    };
}


#endif //XO_SQUAREINFLUENCE_H
