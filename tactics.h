#ifndef XO_TACTICS_H
#define XO_TACTICS_H

#include "branch.h"


namespace XO{
    class Tactics : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            return Scan4SOnly<StaticTactics<>>()(links, own);
        }
    };
}

#endif //XO_TACTICS_H
