#ifndef XO_VARIATIONMANAGER_H
#define XO_VARIATIONMANAGER_H

#include "squareobserver.h"
#include "depth_manager.h"


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
        SquareObserver& m_obs;
        MoveList m_moves;
        DepthManager m_depth_controller;

    public:
        VariationManager(SquareObserver& obs)
            :m_obs(obs), m_depth_controller()
        {}

        void Alloc(DValueT move_count){
            m_moves.reserve(move_count);
        }

        void Reset(){
            m_depth_controller.Reset();
        }

        void SetDepthLimit(DValueT depth_limit){
            m_depth_controller.SetDepthLimit(depth_limit);
        }

        void MakeMove(const AbstractVariation* m){
            m_moves.push_back(m);
            m_obs.NotifySetPiece(*m);

            m_depth_controller.OnNewVariation(GetDepth());
        }

        void TakeBack(const AbstractVariation* m){
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
