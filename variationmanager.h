#ifndef XO_VARIATIONMANAGER_H
#define XO_VARIATIONMANAGER_H

#include <iostream>
#include "squareobserver.h"
#include "depth_manager.h"
#include "baseevaluator.h"


namespace XO{
    class Variation : public Move{
        TProperty m_type;

    public:

        explicit Variation(const Move& m, TProperty type)
            :Move(m), m_type(type)
        {}

        std::string ToString() const{
            std::string name;
            switch(m_type){
                case TProperty::S4:
                    name = "S4";
                    break;
                case TProperty::D3:
                    name = "D3";
                    break;
                default:
                    name = "";
                    break;
            }
            return name + Move::ToString();
        }
    };

    struct PositionData : public PositionCache::IData{
        EvaluationReport data;
    };

    class VariationManager{
    public:
        using MoveList = std::vector<const Variation*>;

    private:
        SquareObserver& m_obs;
        MoveList m_moves;
        DepthManager m_depth_controller;

    public:
        PositionData* FindPositionDataPtr(const Move& m){
            Piece turn = OppositePiece(m.GetTurn());

            auto hook = m_obs.GetPositionCache().Find(turn, m);
            if(!m_obs.GetPositionCache().ValidIter(turn, hook) || !hook->second){
                return nullptr;
            }
            auto data_ptr = static_cast<PositionData*>(&*hook->second);

            if(data_ptr->data.OldDepthReport(GetDepthController().DepthLimit())){
                m_obs.GetPositionCache().Erase(turn, hook);
                return nullptr;
            }

            return data_ptr;
        }

        PositionData* GetPositionDataPtr(Piece p){
            auto hook = m_obs.GetPositionCache().GetCurrent(p);
            if(!hook->second){
                hook->second = std::make_unique<PositionData>();
            }
            return static_cast<PositionData*>(&*hook->second);
        }

        uint64_t PositionCount() const{
            return m_obs.PositionCacheSize();
        }

        VariationManager(SquareObserver& obs)
            :m_obs(obs), m_depth_controller()
        {}

        void Alloc(DValueT move_count){
            m_moves.reserve(move_count);
        }

        void Reset(){
            m_depth_controller.Reset();
            m_obs.GetPositionCache().Clear();
        }

        void SetDepthLimit(DepthT depth_limit){
            m_depth_controller.SetDepthLimit(depth_limit);
        }

        void MakeMove(const Variation* m){
            m_moves.push_back(m);
            m_obs.NotifySetPiece(*m);

            m_depth_controller.OnNewVariation(GetDepth());
            if(m_depth_controller.PullDepthWarning()){
                std::cerr << "(!)Depth " << std::to_string(GetDepth()) << std::endl;
            }
            if(m_depth_controller.PullCounterWarning()){
                std::cerr << "(!)" << ToString() << std::endl;
            }
        }

        void TakeBack(const Variation* m){
            m_obs.NotifyRemovePiece(*m);
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

        DepthT GetDepth() const{
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

    class AutoVariation : public Variation{
        VariationManager& m_manager;

    public:
        AutoVariation(VariationManager& mgr, const Move& m
                , TProperty type = TProperty::ANY)
                :Variation(m, type), m_manager(mgr)
        {
            m_manager.MakeMove(this);
        }

        ~AutoVariation(){
            m_manager.TakeBack(this);
        }
    };
}

#endif //XO_VARIATIONMANAGER_H
