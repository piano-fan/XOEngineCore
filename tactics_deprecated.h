#ifndef XO_TACTICS_DEPRECATED_H
#define XO_TACTICS_DEPRECATED_H

#include "statictactics.h"


namespace XO{
    class Tactics_Deprecated{
        static void MakeSuccessReport(BaseEvaluator::Data& links, Move mv){
            new(&links.result) EvaluationReport(mv
                    , 1
                    , EvaluationReport::Mode::DEPRECATED
                    , EvaluationReport::Type::SUCCESS);
        }

    public:
        class FindForkBlocker : public BaseEvaluator{
        public:
            [[deprecated("Old tactics")]]
            bool operator()(BaseEvaluator::Data& links, Piece defender) const override{
                auto attacker = OppositePiece(defender);
                Point center = links.obs.GetTrackedSquares(attacker, TProperty::FORK).front();
                if(StaticTactics<>::FullBlocker(links.obs, Move(center, defender), TProperty::FORK)){
                    MakeSuccessReport(links, Move(center, defender));
                    return true;
                }
                for(auto current = StarOffset::Begin(); current.Valid(); ++current){
                    Point next = links.obs.Metrics().MakePoint(center, current);
                    if(!links.obs.ValidMove(next)
                       || links.obs.GetThreats(Move(next, attacker))[0].GetTier() < 3  //"cant block"
                       || !StaticTactics<>::FullBlocker(links.obs, Move(next, defender)
                            , TProperty::FORK)){
                        continue;
                    }
                    MakeSuccessReport(links, Move(next, defender));
                    return true;
                }
                return false;
            }
        };

        [[deprecated("Old tactics")]]
        static bool Calculate(BaseEvaluator::Data& links, Piece own){
            auto& forks = links.obs.GetTrackedSquares(OppositePiece(own), TProperty::FORK);
            if(!forks.empty()){
                //TODO: решение проблемы с 2мя угрозами вида:
                //OOO~*X~~~ + #~~X*O~OO - здесь определяются 2 угрозы в открытой троечке + угроза в звездочке
                //        и правый край троечки не распознается как блокер для всех 3х угроз
                //UPD: DONE!

                //TODO: задача решена полностью, но не реализован выбор наилучшего блокера (доказать единственность?)
                //TODO2: решена задача полной ликвидации форсированных угроз
                //      решить проблему выбора наилучшего неполного блокера и найти пример позиции
                //      в которой обычный алгоритм ошибается, а полная блокировка невозможна
                if(forks.size() > 1){
                    if(FindForkBlocker()(links, own)){
                        return true;
                    }
                }
                else{
                    MakeSuccessReport(links, Move(forks.front(), own));
                    return true;
                }
            }
            return false;
        }
    };
}

#endif //XO_TACTICS_DEPRECATED_H
