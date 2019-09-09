#ifndef XO_BRANCH_H
#define XO_BRANCH_H

#include "variationmanager.h"
#include "variationscanner.h"
#include "statictactics.h"
#include "movegenerator.h"


namespace XO{
    template<typename EvAgent>
    class BranchS4 : public BaseEvaluator{
    public:
        bool operator()(BaseEvaluator::Data& links, Piece own) const override{
            auto blocker = links.obs.GetTrackedSquares(own, TProperty::WIN).front();
            auto v_answer = AutoVariation(links.mgr, Move(blocker, OppositePiece(own)));

            EvAgent()(links, own);

            if(links.result.Success()){
                links.result.moves.push_front(v_answer);
            }

            return links.result.Final();
        }
    };

    class S4Variation : public AutoVariation{
    public:
        S4Variation(VariationManager& mgr, const Move& m)
            :AutoVariation(mgr, m)
        {}

        static constexpr TProperty SquareClass(){
            return TProperty::S4;
        }

        std::string ToString() const override{
            return std::string("S4") + AbstractVariation::ToString();
        }
    };

    template<typename EvAgent>
    class Scan4SOnly : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            EvAgent()(links, own)
                || VariationScanner<S4Variation, BranchS4<Scan4SOnly<EvAgent>>>()(links, own);
            return links.result.Final();
        }
    };

    template<typename EvAgent>
    class BranchD3 : public BaseEvaluator{
    public:
        bool operator()(BaseEvaluator::Data& links, Piece own) const override{
            BaseEvaluator::Data counter_links(links);
            Scan4SOnly<StaticTactics<>>()(counter_links, OppositePiece(own));
            if(counter_links.result.Success()){
                return false;
            }

            if(counter_links.result.DepthLimit()){
                links.result = std::move(counter_links.result);
                return false;   //TODO: обрывать перебор при достижении макс. глубины (false)
            }

            EvaluationReport report_candidate;
            D4BlockerGenerator blocks(links.mgr, own);
            for(; blocks.Valid(); blocks.Next()){
                BaseEvaluator::Data next_links{links};
                auto v_answer = AutoVariation(links.mgr, blocks.Get());
                EvAgent()(next_links, own);
                if(!next_links.result.Success()){
                    if(next_links.result.DepthLimit()){
                        links.result = std::move(next_links.result);
                    }
                    return false;
                }

                if(!report_candidate.Final()
                   || next_links.result.FullDepth() > report_candidate.FullDepth()){
                    report_candidate = std::move(next_links.result);
                    report_candidate.moves.push_front(blocks.Get());
                }
            }

            if(blocks.Empty()){
                new(&report_candidate) EvaluationReport(
                        4
                        , EvaluationReport::Mode::DEEP_TACTICS
                        , EvaluationReport::Type::SUCCESS);
            }

            links.result = std::move(report_candidate);
            return links.result.Final();
        }
    };

    class D3Variation : public AutoVariation{
    public:
        D3Variation(VariationManager& mgr, const Move& m)
                :AutoVariation(mgr, m)
        {}

        static constexpr TProperty SquareClass(){
            return TProperty::D3;
        }

        std::string ToString() const override{
            return std::string("D3") + AbstractVariation::ToString();
        }
    };

    template<typename EvAgent>
    class Scan4S3D : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            EvAgent()(links, own)
                || VariationScanner<S4Variation, BranchS4<Scan4SOnly<EvAgent>>>()(links, own)
                || VariationScanner<S4Variation, BranchS4<Scan4S3D<EvAgent>>>()(links, own)
                || VariationScanner<D3Variation, BranchD3<Scan4S3D<EvAgent>>>()(links, own);
            return links.result.Final();
        }
    };
}

#endif //XO_BRANCH_H
