#ifndef XO_VARIATIONMANAGER_H
#define XO_VARIATIONMANAGER_H

#include "squareobserver.h"
#include "positioncache.h"


namespace XO{
    class VariationManager{
    public:
        using MoveList = std::vector<Move>;

    private:
        SquareObserver& m_obs;
        PositionCache m_cache;
        Piece m_own;
        MoveList m_moves;
        DepthT m_root_depth;
        DepthT m_depth_limit;
        DepthT m_max_achieved_depth;

    public:
        VariationManager(SquareObserver& obs)
            :m_obs(obs), m_root_depth(0), m_depth_limit(0)
        {}

        void Alloc(DValueT max_move_count){
            m_moves.reserve(max_move_count);
        }

        void Reset(Piece own){
            m_own = own;
            m_root_depth = m_obs.GetMoveCount();
            m_depth_limit = 0;
            m_cache.Clear();
        }

        void ResetMaxDepthTracker(){
            m_max_achieved_depth = 0;
        }

        DepthT GetDepthLimit() const{
            return m_depth_limit;
        }

        void SetDepthLimit(DepthT depth_limit){
            m_depth_limit = depth_limit;
        }

        void MakeMove(const Move& m){
            m_moves.push_back(m);
            m_obs.NotifySetPiece(m);
            m_max_achieved_depth = std::max(m_max_achieved_depth, GetDepth());
        }

        void TakeBack(const Move& m){
            m_obs.NotifyRemovePiece(m);
            m_moves.pop_back();
        }

        const SquareObserver& GetObserver() const{
            return m_obs;
        }

        PositionCache& GetPositionCache(){
            return m_cache;
        }

        const PositionHash& GetPositionHashRef() const{
            return m_obs.GetPositionHashRef();
        }

        PositionCache::HookT& GetPositionCacheHook(const Move& mv){
            SquareObserver::DirtyPositionHashState destructor(m_obs, mv);
            return GetPositionCache().GetHookByKey(m_obs.GetPositionHashRef());
        }

        const MoveList& Moves() const{
            return m_moves;
        }

        DepthT GetDepth() const{
            return Moves().size();
        }

        DepthT GetMaxAchievedDepth() const{
            return m_max_achieved_depth;
        }

        uint64_t GetPositionCount() const{
            return m_cache.Size();
        }

        bool AtMaxDepth() const{
            return GetDepth() >= m_depth_limit;
        }

        bool IsRoot() const{
            return GetDepth() == 0;
        }

        Piece Own() const{
            return m_own;
        }

        std::string MovesToString() const{
            auto result = std::string()
                          + "Depth " + std::to_string(GetDepth()) + ": ";
            for(auto& move: Moves()){
                result += move.ToString() + " ";
            }
            return result;
        }

        std::string ToString() const{
            std::string result;
            result += m_obs.ToString() + ", ";
            result += MovesToString();
            return result;
        }
    };
}

#endif //XO_VARIATIONMANAGER_H
