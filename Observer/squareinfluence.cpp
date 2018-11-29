#include "squareinfluence.h"


namespace XO{
    void SquareInfluence::UpdateThreats(ValueT dir){
        m_threats[ALLY].Set(Threat::Get(ALLY, GetPieceSet(dir)), dir);
        m_threats[ENEMY].Set(Threat::Get(ENEMY, GetPieceSet(dir)), dir);
    }

    void SquareInfluence::Calculate(){
        for(ValueT dir = 0; dir < 4; ++dir) {
            UpdateThreats(dir);
        }
    }

    void SquareInfluence::OnNeighbourChanged(Piece p, StarOffset self){
        auto dir = self.Angle();
        m_psets[dir].SetPiece(p, self.MirroredOffset());
        UpdateThreats(dir);
    }
}
