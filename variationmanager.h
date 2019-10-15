#ifndef XO_VARIATIONMANAGER_H
#define XO_VARIATIONMANAGER_H

#include "squareobserver.h"
#include "depth_manager.h"
#include "baseevaluator.h"


namespace XO{
    class AbstractVariation : public Move{
    public:
        explicit AbstractVariation(const Move& m)
            :Move(m)
        {}

        virtual std::string ToString() const{
            return Move::ToString();
        }
    };

    class VariationManager{
    public:
        using MoveList = std::vector<const AbstractVariation*>;

    private:
        using ReportCache = std::map<DynamicPieceSet, EvaluationReport>;

        SquareObserver& m_obs;
        DynamicPieceSet& m_hash_key;
        MoveList m_moves;
        DepthManager m_depth_controller;
        mutable ReportCache m_reports;

        ReportCache::const_iterator CurrentCachedReport() const{
            return m_reports.find(m_hash_key);
        }

        bool ValidCachedReport(const ReportCache::const_iterator &iter) const{
            return iter != m_reports.end();
        }

    public:
        bool GetCachedReport(BaseEvaluator::Data &links, const Move &m) const{
            m_hash_key.SetPiece(m);
            auto report_iter = CurrentCachedReport();
            bool valid = ValidCachedReport(report_iter);
            m_hash_key.RemovePiece(m);
            if(valid){
                links.result = (*report_iter).second;
            }
            return valid;
        }

        void WriteCachedReport(const EvaluationReport &result){
            auto report_iter = CurrentCachedReport();
            bool valid = ValidCachedReport(report_iter);
            assert(!valid);
            m_reports[m_hash_key] = result;
        }

        uint64_t PositionCount(){
            return m_reports.size();
        }

        VariationManager(SquareObserver& obs, DynamicPieceSet& hash_key)
            :m_obs(obs), m_hash_key(hash_key), m_depth_controller()
        {}

        void Alloc(DValueT move_count){
            m_moves.reserve(move_count);
        }

        void Reset(){
            m_depth_controller.Reset();
            m_reports.clear();
        }

        void SetDepthLimit(DValueT depth_limit){
            m_depth_controller.SetDepthLimit(depth_limit);
            m_reports.clear();
        }

        void MakeMove(const AbstractVariation* m){
            m_moves.push_back(m);
            m_obs.NotifySetPiece(*m);
            m_hash_key.SetPiece(*m);

            m_depth_controller.OnNewVariation(GetDepth());
        }

        void TakeBack(const AbstractVariation* m){
            m_obs.NotifyRemovePiece(*m);
            m_hash_key.RemovePiece(*m);
            m_moves.pop_back();
        }

        const DepthManager& GetDepthController() const{
            return m_depth_controller;
        }

        const SquareObserver& GetObserver() const{
            return m_obs;
        }

        const MoveList& Moves() const{
            return m_moves;
        }

        DValueT GetDepth() const{
            return Moves().size();
        }

        bool AtMaxDepth() const{
            return m_depth_controller.AtMaxDepth(GetDepth());
        }

        bool IsRoot() const{
            return GetDepth() == 0;
        }

        std::string MovesToString() const{
            auto result = std::string()
                          + "Depth " + std::to_string(GetDepth()) + ": ";
            for(auto& move: Moves()){
                result += move->ToString() + " ";
            }
            return result;
        }

        std::string ToString() const{
            std::string result;
            result += m_depth_controller.ToString() + ", ";
            result += MovesToString();
            return result;
        }
    };

    class AutoVariation : public AbstractVariation{
        VariationManager& m_manager;

    public:
        AutoVariation(VariationManager& mgr, const Move& m)
                :AbstractVariation(m), m_manager(mgr)
        {
            m_manager.MakeMove(this);
        }

        ~AutoVariation(){
            m_manager.TakeBack(this);
        }
    };
}

#endif //XO_VARIATIONMANAGER_H
