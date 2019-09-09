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

    class D4BlockerGenerator : public BaseMoveGenerator{
        StarOffset m_current_offset;
        Point m_center;
        Piece m_attacker;
        Move m_current;
        bool m_empty;
        bool m_valid;

        bool CurrentIsOK() const{
            return StaticTactics<>::FullBlocker(Obs(), m_current, TProperty::WIN)
                   && StaticTactics<>::FullBlocker(Obs(), m_current, TProperty::D4);
        }

        void Find(){
            for(; m_current_offset.Valid(); ++m_current_offset){
                m_current = Move(Obs().Metrics().MakePoint(m_center, m_current_offset)
                        , OppositePiece(m_attacker));
                if(Obs().ValidMove(m_current) && CurrentIsOK()){
                    ++m_current_offset;
                    return;//TODO: заменить FULL BLOCKER на более простое условие напр S4?
                }
            }
            m_valid = false;
        }

    public:
        D4BlockerGenerator(VariationManager& manager, Piece attacker)
                :BaseMoveGenerator(manager), m_current_offset(StarOffset::Begin())
                , m_attacker(attacker), m_empty(false), m_valid(true)
        {
            Reset();
            m_empty = !Valid();
        }

        void Reset() override{
            auto& s5s = Obs().GetTrackedSquares(m_attacker, TProperty::WIN);
            if(s5s.size() >= 2){
                m_valid = false;
                return;
            }
            if(s5s.size() == 1){
                m_current = Move(s5s.front(), OppositePiece(m_attacker));
                m_current_offset = StarOffset::End();
                m_valid = CurrentIsOK();
                return;
            }
            m_valid = true;
            m_current_offset = StarOffset::Begin();
            assert(!Obs().GetTrackedSquares(m_attacker, TProperty::D4).empty());
            m_center = Obs().GetTrackedSquares(m_attacker, TProperty::D4).front();
            m_current = Move(m_center, OppositePiece(m_attacker));
            if(!CurrentIsOK()){
                Find();
            }
        }

        void Next() override{
            assert(Valid());
            Find();
        }

        Move Get() const override{
            return m_current;
        }

        bool Valid() const override{
            return m_valid;
        }

        bool Empty() const override{
            return m_empty;
        }
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
            if(SquareClass == TProperty::S4){
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
            }
            else if(SquareClass == TProperty::D3){
                if(mgr.GetObserver().GetTrackedSquares(OppositePiece(own), TProperty::D4).empty()){
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
                }
                else{
                    D4BlockerGenerator blocks(mgr, OppositePiece(own));
                    for(; blocks.Valid(); blocks.Next()){
                        auto mv = Move(blocks.Get().GetPos(), own);
                        if(mgr.GetObserver().GetTactics(mv).GetProperty(SquareClass)){
                            m_points.push_back(mv.GetPos());
                        }
                    }
                }
            }
            else{
                throw(std::logic_error("TrackedMoveGenerator"));
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
