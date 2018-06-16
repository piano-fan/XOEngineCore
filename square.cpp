#include <cassert>
#include "square.h"
#include "field.h"


namespace XO{
    Square::Square(Field* parent):
            m_field(parent), m_piece(EMPTY)
    {}

    void Square::Init(int x, int y){
        m_x = x;
        m_y = y;

        for(int dir = 0; dir < 4; dir++){
            m_mask_cache[dir] = Iter().MakePiecemask8(dir);
        }
    }

    Piece Square::GetPiece() const{
        return m_piece;
    }

    Piece Square::PlacePiece(Piece p) {
        Piece current = m_piece;
        assert(current == EMPTY);
        m_piece = p;

        FieldIterator iter(m_field, m_x, m_y);
        for(int dir = 0; dir < 4; dir++){
            FieldIterator first = iter;
            FieldIterator second = first;
            for(int i = 0; i < 4; i++){
                if(first.Step(dir, true).GetPiece() == EMPTY){
                    (*first).m_mask_cache[dir].SetPiece(p, 4 + i);
                    (*first).m_sqinfo.m_modified = 1;
                }
                if(second.Step(dir, false).GetPiece() == EMPTY) {
                    (*second).m_mask_cache[dir].SetPiece(p, 3 - i);
                    (*second).m_sqinfo.m_modified = 1;
                }
            }
        }

        return current;
    }

    Piece Square::ClearPiece() {
        Piece current = m_piece;
        assert(current == X || current == O);
        m_piece = EMPTY;

        FieldIterator iter(m_field, m_x, m_y);
        for(int dir = 0; dir < 4; dir++){
            FieldIterator first = iter;
            FieldIterator second = first;
            for(int i = 0; i < 4; i++){
                if(first.Step(dir, true).GetPiece() == EMPTY){
                    (*first).m_mask_cache[dir].ClearPiece(4 + i);
                    (*first).m_sqinfo.m_modified = 1;
                }
                if(second.Step(dir, false).GetPiece() == EMPTY) {
                    (*second).m_mask_cache[dir].ClearPiece(3 - i);
                    (*second).m_sqinfo.m_modified = 1;
                }
            }
        }

        return current;
    }

    PieceMask8 Square::GetPieceMask(int dir) const {
        return m_mask_cache[dir];
    }

    bool Square::IsWin(Piece p) const{
        for(int dir = 0; dir < 4; dir++){
            if(GetPieceMask(dir).IsWin(p)){
                return true;
            }
        }
        return false;
    }

    int Square::GetValue(Piece p) const {
        if(m_sqinfo.m_modified){
            UpdateSqInfo();
        }
        return m_sqinfo.m_value[p];
    }

    FieldIterator Square::Iter() const{
        return FieldIterator(m_field, m_x, m_y);
    }

    void Square::UpdateSqInfo() const {
        for (Piece &p: std::array<Piece, 2>{X, O}){
            ValueT tier = NO_THREAT;
            ValueT score = 0;
            bool single_win_threat = false;
            for (int dir = 0; dir < 4; dir++) {
                const auto& nextinfo = ThreatCache::Instance()[GetPieceMask(dir)];
                if(nextinfo.GetCRef(p, SINGLE_THREAT).GetTier() == WIN){
                    tier = WIN;
                    score = FINISHER_SCORE;
                    break;
                }

                if(single_win_threat && nextinfo.GetCRef(p, DUAL_THREAT).GetTier() == PRE_WIN_THREAT){
                    tier = PRE_WIN_THREAT;
                    score = FINISHER_SCORE;
                }
                if(nextinfo.GetCRef(p, SINGLE_THREAT).GetTier() == WIN_THREAT){
                    if(single_win_threat){
                        tier = WIN_THREAT;
                        score = FINISHER_SCORE;
                    }
                    else {
                        single_win_threat = true;
                        if (tier == PRE_WIN_THREAT) {
                            score = FINISHER_SCORE;
                        }
                    }
                }

                ThreatInfo next = nextinfo.GetCRef(p, DUAL_THREAT);
                if(tier < next.GetTier()){
                    tier = next.GetTier();
                    score = next.GetScore();
                }
                else if(tier == next.GetTier()) {
                    score += next.GetScore();
                    if (tier == PRE_WIN_THREAT) {
                        score = FINISHER_SCORE;
                    }
                }
            }
            m_sqinfo.m_value[p] = score + (tier << 5);
        }
        m_sqinfo.m_modified = 0;
    }

    //TODO: решение проблемы с 2мя угрозами вида:
    //OOO~*X~~~ + #~~X*O~OO - здесь определяются 2 угрозы в открытой троечке + угроза в звездочке
    //        и правый край троечки не распознается как блокер для всех 3х угроз

    std::string Square::ToString() const {
        std::string result = "";
        for(auto &mask: m_mask_cache){
            result += mask.ToString() + "\n";
        }
        result += "Value X: " + std::to_string(GetValue(X)) + "\n";
        result += "Value O: " + std::to_string(GetValue(O)) + "\n";
        return result;
    }
}
