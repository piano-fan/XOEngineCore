#ifndef XO_STATICTACTICS_H
#define XO_STATICTACTICS_H

#include "baseevaluator.h"


namespace XO{
    class StaticTactics : public BaseEvaluator{
        static void MakeReport(Data& links, const Move& mv, DValueT static_depth
                                , EvaluationReport::Type type){
            new(&links.result) EvaluationReport(mv
                            , static_depth
                            , EvaluationReport::Mode::STATIC_TACTICS
                            , type);
        }

    public:
        static bool Blocks(const SquareObserver& obs, const Move& m,
                           StarOffset target, TacticSet to_block){
            assert(target.Valid());
            auto target_point = obs.Metrics().MakePoint(m.GetPos(), target);
            assert(obs.ValidMove(target_point));
            assert(obs.GetTactics(Move(target_point, OppositePiece(m.GetTurn()))).AnyOf(to_block));
            auto influence = obs.GetInfluence(target_point);
            if(influence.GetThreats(OppositePiece(m.GetTurn())).GetRank(target.Angle()) > 1){
                return false;
            }
            influence.OnNeighbourChanged(m.GetTurn(), target);
            return !influence.GetTactics(OppositePiece(m.GetTurn())).AnyOf(to_block);
        }

        static bool FullBlocker(const SquareObserver &obs, const Move &m
                , TProperty to_block){
            for(auto& sq: obs.GetTrackedSquares(OppositePiece(m.GetTurn()), to_block)){
                if(m.GetPos() == sq){
                    continue;
                }
                auto offset = obs.Metrics().MakeOffset(m.GetPos(), sq);
                if(!offset.Valid()){
                    return false;
                }
                if(!StaticTactics::Blocks(obs, m, offset, to_block)){
                    return false;
                }
            }
            return true;
        }

        bool operator()(Data& links, Piece own) const override{
            //Instant win check
            auto sqlist = &links.obs.GetTrackedSquares(own, TProperty::WIN);
            if(!sqlist->empty()){
                MakeReport(links, Move(sqlist->front(), own), 1, EvaluationReport::Type::SUCCESS);
                return true;
            }

            //Instant loss check
            sqlist = &links.obs.GetTrackedSquares(OppositePiece(own), TProperty::WIN);
            auto count = sqlist->size();
            if(count >= 2){
                MakeReport(links, Move(sqlist->front(), own), 2, EvaluationReport::Type::FAIL);
                return true;
            }
            if(count == 1){
                MakeReport(links, Move(sqlist->front(), own), 1, EvaluationReport::Type::NONE);
                return true;
            }

            //Simple fork check
            sqlist = &links.obs.GetTrackedSquares(own, TProperty::D4);
            if(!sqlist->empty()){
                MakeReport(links, Move(sqlist->front(), own), 3, EvaluationReport::Type::SUCCESS);
                return true;
            }

            sqlist = &links.obs.GetTrackedSquares(own, TProperty::D3_D3);
            if(!sqlist->empty()){
                auto enemy_t4_list = &links.obs.GetTrackedSquares(OppositePiece(own)
                        , TProperty::S4);
                if(enemy_t4_list->empty()){
                    MakeReport(links, Move(sqlist->front(), own), 5, EvaluationReport::Type::SUCCESS);
                    return true;
                }
            }

            return false;
        }
    };
}

#endif //XO_STATICTACTICS_H
