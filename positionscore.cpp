#include "positionscore.h"
#include "core.h"
#include <cassert>


namespace XO{
    void PositionScore::Calculate(XO::Core &core, Piece p) {
        Point top = core.m_state.GetMetrics().Middle();
        int bestvalue = 0;
        if(core.m_state.GetField()[top] == EMPTY){
            bestvalue = core.m_square_cache.Get(top, p).Value();
        }

        for(auto &sq: core.m_state.GetMetrics()){
            if(core.m_state.GetField()[sq] != EMPTY){
                continue;
            }
            SquareScore nextscore = core.m_square_cache.Get(sq, p);

            if(bestvalue < nextscore.Value()){
                top = sq;
                bestvalue = nextscore.Value();
            }
        }
        m_top_scored[p] = top;
    }

    void PositionScore::Calculate(Core& core) {
        Calculate(core, X);
        Calculate(core, O);

        Piece own = core.m_state.GetTurn();
        Piece opp = OppositePiece(own);

        if(core.m_square_cache.Get(m_top_scored[own], own).Value()
                >= core.m_square_cache.Get(m_top_scored[opp], opp).Value()){
            m_leader = own;
        }
        else{
            m_leader = opp;
        }

        m_valid = true;
    }
}