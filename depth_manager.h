#ifndef XO_DEPTH_MANAGER_H
#define XO_DEPTH_MANAGER_H

#include "builtintypes.h"


namespace XO{
    class DepthManager{
        uint64_t m_child_counter;
        DepthT m_highest_depth;
        DepthT m_depth_limit;
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
            m_depth_warning = false;
            m_counter_warning = false;
        }

        void SetDepthLimit(DepthT depth_limit){
            m_depth_limit = depth_limit;
        }

        void OnNewVariation(DepthT new_depth){
            ++m_child_counter;
            if(new_depth > m_highest_depth){
                m_highest_depth = new_depth;
                if(m_highest_depth > 30){
                    m_depth_warning = true;
                }
            }
            if(!(m_child_counter % 1000000)){
                m_counter_warning = true;
            }
        }

        uint64_t ChildCount() const{
            return m_child_counter;
        }

        DepthT HighestDepth() const{
            return m_highest_depth;
        }

        DepthT DepthLimit() const{
            return m_depth_limit;
        }

        bool AtMaxDepth(DepthT current_depth) const{
            return current_depth >= DepthLimit();
        }

        bool PullDepthWarning(){
            if(m_depth_warning){
                m_depth_warning = false;
                return true;
            }
            return false;
        }

        bool PullCounterWarning(){
            if(m_counter_warning){
                m_counter_warning = false;
                return true;
            }
            return false;
        }

        std::string ToString() const{
            return std::string()
                   + "Childs " + std::to_string(m_child_counter);
        }
    };
}

#endif //XO_DEPTH_MANAGER_H
