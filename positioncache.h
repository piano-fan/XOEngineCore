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

        ContainerT m_data;

    public:
        HookT& GetHookByKey(const KeyT& key){
            return m_data[key];
        }

        void Clear(){
            m_data.clear();
        }

        uint64_t Size() const{
            return m_data.size();
        }
    };
}

#endif //XO_POSITIONCACHE_H
