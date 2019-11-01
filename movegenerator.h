#ifndef XO_MOVEGENERATOR_H
#define XO_MOVEGENERATOR_H

#include "types.h"
#include "statictactics.h"


namespace XO{
    class MoveGenerator{
        class BaseHelper{
        protected:
            const SquareObserver* m_obs;
            Piece m_turn;
            TProperty m_square_class;
            bool m_empty;
            bool m_valid;
            Point m_current;

            BaseHelper() = default;

            BaseHelper(const SquareObserver& obs, Piece turn, TProperty sq_class)
                    :m_obs(&obs), m_turn(turn), m_square_class(sq_class)
                    , m_empty(true), m_valid(false)
            {}

        public:
            Point Get() const{
                return m_current;
            }

            Move GetMove() const{
                return Move(m_current, m_turn);
            }

            bool Valid() const{
                return m_valid;
            }

            bool Empty() const{
                return m_empty;
            }

            virtual void Reset() = 0;
            virtual void Next() = 0;
        };

        class SingleMoveHelper : public BaseHelper{
        public:
            SingleMoveHelper() = default;

            SingleMoveHelper(const SquareObserver& obs, const Move& mv, TProperty sq_class)
                    :BaseHelper(obs, mv.GetTurn(), sq_class)
            {
                m_current = mv.GetPos();
                bool indirect_d3_reload = (sq_class == TProperty::D3)   //TODO: not only D3
                                    && m_obs->HaveSquares(GetMove().GetTurn(), TProperty::D4);
                m_empty = (!m_obs->GetTactics(GetMove()).GetProperty(m_square_class)
                              && !indirect_d3_reload)
                          || (m_square_class != TProperty::S4
                              && !StaticTactics::FullBlocker(*m_obs, GetMove(), TProperty::D4));
                Reset();
            }

            void Reset() override{
                m_valid = !m_empty;
            }

            void Next() override{
                m_valid = false;
            }
        };

        class UnlimitedHelper : public BaseHelper{
            using iterator = SquareTracker_Container::const_iterator;
            iterator m_current_offset;
            iterator m_end;
        public:
            UnlimitedHelper(const SquareObserver& obs, Piece turn, TProperty sq_class)
                    :BaseHelper(obs, turn, sq_class)
            {
                Reset();
            }

            void Reset() override{
                auto& sqlist = m_obs->GetTrackedSquares(m_turn, m_square_class);
                m_empty = sqlist.empty();
                m_valid = !m_empty;
                if(!m_empty){
                    m_current_offset = sqlist.begin();
                    m_end = sqlist.end();
                    m_current = *m_current_offset;
                }
            }

            void Next() override{
                ++m_current_offset;
                if(m_current_offset == m_end){
                    m_valid = false;
                }
                else{
                    m_current = *m_current_offset;
                }
            }
        };

        class D4BlockerHelper : public BaseHelper{
            Point m_center;
            bool m_at_center;
            StarOffset m_offset = StarOffset::Begin();

            bool CurrentIsOK() const{
                return m_obs->GetTactics(GetMove()).GetProperty(m_square_class)
                       && StaticTactics::FullBlocker(*m_obs, GetMove(), TProperty::D4);
            }
        public:
            D4BlockerHelper(const SquareObserver& obs, Piece turn, TProperty sq_class)
                    :BaseHelper(obs, turn, sq_class)
            {
                m_center = m_obs->AnySquare(OppositePiece(m_turn), TProperty::D4);
                Reset();
                m_empty = !m_valid;
            }

            void Reset() override{
                m_offset = StarOffset::Begin();
                m_current = m_center;
                m_at_center = true;
                if(!CurrentIsOK()){
                    Next();
                }
                else{
                    m_valid = true;
                    m_empty = false;
                }
            }

            void Next() override{
                if(m_at_center){
                    m_at_center = false;
                }
                else{
                    ++m_offset;
                }

                for(; m_offset.Valid(); ++m_offset){
                    m_current = m_obs->Metrics().MakePoint(m_center, m_offset);
                    if(m_obs->ValidMove(GetMove())
                       && CurrentIsOK()){
                        m_valid = true;
                        m_empty = false;
                        return;
                    }
                }

                m_valid = false;
            }
        };

        union MixedHelper{
            SingleMoveHelper single;
            UnlimitedHelper full;
            D4BlockerHelper d4;

            MixedHelper()
                :single()
            {}
        };

        MixedHelper helpers;
        BaseHelper* active_helper;

    public:
        MoveGenerator(const SquareObserver& obs, Piece turn, TProperty square_class){
            assert(!StaticTactics::InstantLoss(obs, turn));
            if(Move mv; StaticTactics::SingleMove(mv, obs, turn)){
                new(&helpers.single) SingleMoveHelper(obs, mv, square_class);
                active_helper = &helpers.single;
            }
            else if(square_class != TProperty::S4
                && obs.HaveSquares(OppositePiece(turn), TProperty::D4)){
                new(&helpers.d4) D4BlockerHelper(obs, turn, square_class);
                active_helper = &helpers.d4;
            }
            else{
                new(&helpers.full) UnlimitedHelper(obs, turn, square_class);
                active_helper = &helpers.full;
            }
        }

        Point Get() const{
            return active_helper->Get();
        }

        Move GetMove() const{
            return active_helper->GetMove();
        }

        bool Valid() const{
            return active_helper->Valid();
        }

        bool Empty() const{
            return active_helper->Empty();
        }

        virtual void Reset(){
            active_helper->Reset();
        }
        virtual void Next(){
            active_helper->Next();
        }
    };
}

#endif //XO_MOVEGENERATOR_H
