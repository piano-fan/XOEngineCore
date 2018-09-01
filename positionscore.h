#ifndef XO_POSITIONSCORE_H
#define XO_POSITIONSCORE_H

#include "types.h"


namespace XO{
    class Core;

    class PositionScore{
        Point m_top_scored[2];
        Piece m_leader;
        bool m_valid;

        void Calculate(Core& core, Piece p);
    public:
        PositionScore():
                m_valid(false)
        {}

        void Calculate(Core& core);
        bool Valid() const{
            return m_valid;
        }
        void Invalidate(){
            m_valid = false;
        }

        Point GetDumbBestMove(Piece turn) const{
            return m_top_scored[m_leader];
        }
    };
}

#endif //XO_POSITIONSCORE_H
