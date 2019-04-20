#ifndef XO_MOVEGENERATOR_H
#define XO_MOVEGENERATOR_H

#include "types.h"
#include "variationmanager.h"
#include "statictactics.h"


namespace XO{
    class BaseMoveGenerator{
        const VariationManager& m_manager;

    protected:
        const SquareObserver& Obs() const{
            return m_manager.GetObserver();
        }

    public:
        BaseMoveGenerator(const VariationManager& mgr)
            :m_manager(mgr)
        {}

        virtual void Reset() = 0;
        virtual void Next() = 0;
        virtual Move Get() const = 0;
        virtual bool Valid() const = 0;
        virtual bool Empty() const = 0;
    };

    template<TProperty SquareClass>
    class TrackedMoveGenerator : public BaseMoveGenerator{
        SquareTracker_Container m_points;
        SquareTracker_Container::const_iterator m_current;
        Piece m_own;
    public:
        TrackedMoveGenerator(VariationManager& mgr, Piece own)
            :BaseMoveGenerator(mgr), m_own(own), m_points(), m_current()
        {
            auto& opp_threats = mgr.GetObserver().GetTrackedSquares(
                    OppositePiece(own), TProperty::WIN);
            if(opp_threats.size() > 1){}
            else if(opp_threats.size() == 1){
                auto mv = Move(opp_threats.front(), own);
                if(mgr.GetObserver().GetTactics(mv).GetProperty(SquareClass)){
                    m_points.push_back(opp_threats.front());
                }
            }
            else{
                m_points = mgr.GetObserver().GetTrackedSquares(own, SquareClass);
            }
            m_current = m_points.begin();
        }

        void Reset() override{
            m_current = m_points.begin();
        }

        void Next() override{
            assert(Valid());
            ++m_current;
        }

        Move Get() const override{
            return Move(*m_current, m_own);
        }

        bool Valid() const override{
            return m_current != m_points.end();
        }

        bool Empty() const override{
            return m_points.empty();
        }
    };
}

#endif //XO_MOVEGENERATOR_H
