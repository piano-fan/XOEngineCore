#ifndef XO_TACTICS_DEPRECATED_H
#define XO_TACTICS_DEPRECATED_H

#include "statictactics.h"


namespace XO{
    class Tactics_Deprecated{
    public:
        class FindForkBlocker{
        public:
            [[deprecated("Old tactics")]]
            bool operator()(Move& r_result, const SquareObserver& obs, Piece defender) const{
                auto attacker = OppositePiece(defender);
                Point center = obs.GetTrackedSquares(attacker, TProperty::FORK).front();
                if(StaticTactics::FullBlocker(obs, Move(center, defender), TProperty::FORK)){
                    r_result = Move(center, defender);
                    return true;
                }
                for(auto current = StarOffset::Begin(); current.Valid(); ++current){
                    Point next = obs.Metrics().MakePoint(center, current);
                    if(!obs.ValidMove(next)
                       || obs.GetThreats(Move(next, attacker))[0].GetTier() < 3  //"cant block"
                       || !StaticTactics::FullBlocker(obs, Move(next, defender)
                            , TProperty::FORK)){
                        continue;
                    }
                    r_result = Move(next, defender);
                    return true;
                }
                return false;
            }
        };

        [[deprecated("Old tactics")]]
        bool operator()(Move& r_result, const SquareObserver& obs, Piece own) const{
            auto opp = OppositePiece(own);
            unsigned int bestvalue[2] = {0, 0};
            Point top_scored[2];

            for(auto &p: Players){
                for(auto &sq: obs.GetTrackedSquares(p, TProperty::FORK)){
                    auto nextscore = obs.GetTactics(Move(sq, p));

                    if(bestvalue[p] < nextscore.Value()){
                        top_scored[p] = sq;
                        bestvalue[p] = nextscore.Value();
                    }
                }
            }
            if(bestvalue[own] == 0 && bestvalue[opp] == 0){
                return false;
            }
            if(bestvalue[own] >= bestvalue[opp]){
                r_result = Move(top_scored[own], own);//TODO: remove
                return true;
            }
            else{
                //TODO: решение проблемы с 2мя угрозами вида:
                //OOO~*X~~~ + #~~X*O~OO - здесь определяются 2 угрозы в открытой троечке + угроза в звездочке
                //        и правый край троечки не распознается как блокер для всех 3х угроз
                //UPD: DONE!

                //TODO: задача решена полностью, но не реализован выбор наилучшего блокера (доказать единственность?)
                //TODO2: решена задача полной ликвидации форсированных угроз
                //      решить проблему выбора наилучшего неполного блокера и найти пример позиции
                //      в которой обычный алгоритм ошибается, а полная блокировка невозможна
                auto& forks = obs.GetTrackedSquares(opp, TProperty::FORK);
                if(forks.size() > 1){
                    if(FindForkBlocker()(r_result, obs, own)){
                        return true;
                    }
                }
                else{
                    r_result = Move(forks.front(), own);
                    return true;
                }
                return false;
            }
        }
    };
}

#endif //XO_TACTICS_DEPRECATED_H
