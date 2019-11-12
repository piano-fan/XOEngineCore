#ifndef XO_POSITIONCACHE_H
#define XO_POSITIONCACHE_H

#include "types.h"
#include "pieceset.h"


namespace XO{
    class PositionCache{
    public:
        class IData{
        public:
            virtual ~IData(){}
        };

        using KeyT = DynamicPieceSet;
        using HookT = std::unique_ptr<IData>;
        using ContainerT = std::map<KeyT, HookT>;
        using IterT = ContainerT::iterator;

    private:
        std::array<ContainerT, 2> m_data;
        KeyT m_current_key;

        friend class SquareObserver;

        void OnSetPiece(const Move& m){
            m_current_key.SetPiece(m);
        }

        void OnRemovePiece(const Move& m){
            m_current_key.RemovePiece(m);
        }

        void OnNewGame(){
            Clear();
            m_current_key.Clear();
        }

        void OnResize(ValueT w, ValueT h){
            Clear();
            m_current_key.Resize(w * h);
        }

        class TemporaryState{
            PositionCache& m_cache;
            const Move m_change;
        public:
            TemporaryState(PositionCache& cache, const Move& change)
                :m_cache(cache), m_change(change)
            {
                m_cache.OnSetPiece(m_change);
            }

            ~TemporaryState(){
                m_cache.OnRemovePiece(m_change);
            }
        };

    public:
        IterT FindCurrent(Piece p){
            return m_data[p].find(m_current_key);
        }

        IterT Find(Piece p, const Move& m){
            TemporaryState tmp(*this, m);
            return FindCurrent(p);
        }

        bool ValidIter(Piece p, IterT iter) const{
            return iter != m_data[p].end();
        }

        IterT GetCurrent(Piece p){
            return m_data[p].insert(std::pair<KeyT, HookT>(m_current_key, nullptr)).first;
        }

        IterT Get(Piece p, const Move& m){
            TemporaryState tmp(*this, m);
            return GetCurrent(p);
        }

        void Erase(Piece p, const IterT& target){
            m_data[p].erase(target);
        }

        void Clear(){
            m_data[0].clear();
            m_data[1].clear();
        }

        uint64_t Size() const{
            return m_data[0].size() + m_data[1].size();
        }
    };
}

#endif //XO_POSITIONCACHE_H
