#ifndef XO_POSITIONCACHE_H
#define XO_POSITIONCACHE_H

#include "types.h"
#include "positionhash.h"


namespace XO{
    struct PositionData;
    class PositionCache{
    public:
        using HookT = std::shared_ptr<PositionData>;

    private:
        using KeyT = PositionHash;
        using ContainerT = std::map<KeyT, HookT>;
        using IterT = ContainerT::iterator;

        ContainerT m_data[2];

    public:
        HookT& GetHookByKey(Piece p, const KeyT& key){
            return m_data[p][key];
        }

        void Clear(){
            m_data[ALLY].clear();
            m_data[ENEMY].clear();
        }

        uint64_t Size() const{
            return m_data[ALLY].size() + m_data[ENEMY].size();
        }
    };
}

#endif //XO_POSITIONCACHE_H
