#ifndef XO_DEPTH_MANAGER_H
#define XO_DEPTH_MANAGER_H

#include "builtintypes.h"


namespace XO{
    class DepthManager{
        uint64_t m_child_counter;
        DValueT m_highest_depth;
        DValueT m_depth_limit;
        bool m_depth_warning;
        bool m_counter_warning;

    public:
        DepthManager()
                :m_child_counter(0), m_highest_depth(0), m_depth_limit(0)
                , m_depth_warning(false), m_counter_warning(false)
        {}

        void Reset(){
            m_child_counter = 0;
            m_highest_depth = 0;
            m_depth_limit = 0;
        }

        void SetDepthLimit(DValueT depth_limit){
            m_depth_limit = depth_limit;
        }

        void OnNewVariation(DValueT new_depth){
            ++m_child_counter;
            if(new_depth > m_highest_depth){
                m_highest_depth = new_depth;
            }
        }

        uint64_t ChildCount() const{
            return m_child_counter;
        }

        DValueT HighestDepth() const{
            return m_highest_depth;
        }

        DValueT DepthLimit() const{
            return m_depth_limit;
        }

        bool AtMaxDepth(DValueT current_depth) const{
            return current_depth >= DepthLimit();
        }

        std::string ToString() const{
            return std::string()
                   + "Childs " + std::to_string(m_child_counter);
        }
    };
}

#endif //XO_DEPTH_MANAGER_H
