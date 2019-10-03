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

            if(!links.mgr.GetCachedReport(links, Move(blocker, OppositePiece(own)))){
                auto v_answer = AutoVariation(links.mgr, Move(blocker, OppositePiece(own)));
                EvAgent()(links, own);
            }

            if(links.result.Success()){
                links.result.moves.push_front(Move(blocker, OppositePiece(own)));
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
            if(links.result.Final()){
                links.mgr.WriteCachedReport(links.result);
            }
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
                if(!links.mgr.GetCachedReport(next_links, blocks.Get())){
                    auto v_answer = AutoVariation(links.mgr, blocks.Get());
                    EvAgent()(next_links, own);
                }

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

            TrackedMoveGenerator<TProperty::S4> generator(links.mgr, OppositePiece(own));
            for(; generator.Valid(); generator.Next()){
                BaseEvaluator::Data blocker_links(links);
                //TODO: придумать как вынести отдельной функцией
                auto v_start = AutoVariation(links.mgr, generator.Get());
                StaticTactics<>()(blocker_links, own);
                if(blocker_links.result.Success()){
                    continue;
                }
                auto blocker = links.obs.GetTrackedSquares(OppositePiece(own), TProperty::WIN).front();
                auto v_answer = AutoVariation(links.mgr, Move(blocker, own));
                //ODOT:
                StaticTactics<>()(blocker_links, OppositePiece(own));
                if(blocker_links.result.Success()){
                    return false;
                }
                if(!links.obs.GetTrackedSquares(own, TProperty::WIN).empty()){
                    BranchS4<EvAgent>()(blocker_links, own);
                }
                else if(!links.obs.GetTrackedSquares(own, TProperty::D4).empty()){
                    BranchD3<EvAgent>()(blocker_links, own);
                }
                else{
                    return false;
                }
                if(!blocker_links.result.Success()){
                    return false;
                }
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
            links.mgr.WriteCachedReport(links.result);
            return links.result.Final();
        }
    };
}

#endif //XO_BRANCH_H
