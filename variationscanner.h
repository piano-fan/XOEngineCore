#ifndef XO_VARIATIONSCANNER_H
#define XO_VARIATIONSCANNER_H

#include "movegenerator.h"
#include "variation.h"

namespace XO{
    static void CloneChildData(PositionData& dst, const Variation& var){
        dst.m_report = var.GetData().m_report;
        dst.m_best_move = var;
        dst.m_best_child = &var.GetData();
        dst.m_scan_depth = var.GetData().m_scan_depth + 1;
    }

    static void MakeDepthLimitReport(PositionData& dst, VariationManager& mgr){
        dst.m_report = Report(
            Report::Author::STATIC_TACTICS
            , Report::Type::DEPTH_LIMIT
            , Move()
            , mgr.GetDepthLimit());
        dst.SetStatus(Move(), nullptr);
    }

    class ForcedWinCalculator : public IVariationCalculator{
    public:
        void Calculate(PositionData& target, VariationManager& mgr, Piece turn) const override{
            if(TryStaticTactics(target, mgr, turn)
              || TryDepthLimit(target, mgr, turn)){
                return;
            }
            TryScanVariations(target, mgr, turn);
        }

    private:
        bool TryStaticTactics(PositionData& target, VariationManager& mgr, Piece turn) const{
            if(!target.m_report){
                StaticTactics()(target.m_report, mgr.GetObserver(), turn);
                target.SetStatus(target.m_report.move, nullptr);
            }
            return target.m_report.Final();
        }

        bool TryDepthLimit(PositionData& target, VariationManager& mgr, Piece turn) const{
            if(!mgr.AtMaxDepth()){
                return false;
            }
            if(!MoveGenerator(mgr.GetObserver(), turn, TProperty::S4).Empty()
             || !MoveGenerator(mgr.GetObserver(), turn, TProperty::D3).Empty()){
                MakeDepthLimitReport(target, mgr);
            }
            //TODO: добавить явный репорт об отсутствии вариаций
            return true;
        }

        void TestVariation(PositionData& target, VariationManager& mgr, const Move& mv) const;

        void TryScanVariations(PositionData& target, VariationManager& mgr, Piece turn) const{
            for(auto& sqclass: {TProperty::S4}){
                std::list<Point> targets[4];
                for(auto gen = MoveGenerator(mgr.GetObserver(), turn, sqclass)
                    ; gen.Valid(); gen.Next()){
                    auto tier = mgr.GetObserver().GetThreats(gen.GetMove())[1].GetTier();
                    targets[tier].push_back(gen.Get());
                }
                for(auto& builder_tier: {3, 2, 1, 0})
                for(auto& sq: targets[builder_tier]){
                    TestVariation(target, mgr, Move(sq, turn));
                    if(target.m_report.Success()){
                        return;
                    }
                }
            }
        };
    };

    class ForcedReactionCalculator : public IVariationCalculator{
    public:
        void Calculate(PositionData& target, VariationManager& mgr, Piece turn) const override{
            Piece attacker = OppositePiece(turn);
            if(mgr.GetObserver().SquareCount(attacker, TProperty::WIN) == 1){
                BranchS4(target, mgr, turn);
                return;
            }
        }

    private:
        void BranchS4(PositionData& target, VariationManager& mgr, Piece turn) const{
            auto blocker = mgr.GetObserver().AnySquare(OppositePiece(turn), TProperty::WIN);

            Variation var(ForcedWinCalculator(), mgr, Move(blocker, turn));

            CloneChildData(target, var);
        }
    };

    void ForcedWinCalculator::TestVariation(PositionData& target, VariationManager& mgr
                       , const Move& mv) const{
        Variation var(ForcedReactionCalculator(), mgr, mv);

        if(var.GetData().m_report.Success()
          || (!target.m_report.DepthLimit() && var.GetData().m_report.DepthLimit())){
            CloneChildData(target, var);
        }
    };
}


#endif //XO_VARIATIONSCANNER_H
