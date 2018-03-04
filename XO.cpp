#include "XO.h"
#include <cassert>
#include <algorithm>
#include <ctime>

namespace XO {
    void Init(){
        ThreatInfo::Init();
    }

    void SquareInfo::Calculate(Piece owner){
        PieceID id = GetPieceID(owner);

        m_ThreatInfo[id] = ThreatInfo();
        for(auto &mask: m_masks){
            m_ThreatInfo[id] += ThreatInfo(mask, owner).Normalize();
        }

        m_FinisherTier[id] = GetFinisherTierSlow(owner);
        m_PieceDistance[id] = GetPieceDistanceSlow(owner);
    }


    SquareInfo::SquareInfo(Field* f, int x, int y)
    {
        FieldIterator sq(f, x, y);
        for(int i = 0; i < 4; i++){
            m_masks[i] = sq.MakePiecemask8(i);
        }
        Calculate(X);
        Calculate(O);

        m_Tactical = GetFinisherTier(X) <= PRE_WIN_THREAT || GetFinisherTier(O) <= PRE_WIN_THREAT;
    }


    std::string SquareInfo::ToString(){
        std::string result = "";
        for(auto &mask: m_masks){
            result += mask.ToString() + "\n";
            result += "X: " + ThreatInfo(mask, X).ToString() + "\n";
            result += "O: " + ThreatInfo(mask, O).ToString() + "\n";
        }
        return result;

        /*std::cout << "Total: " << std::endl;  //OLD CODE TLDR
        std::cout << "X: ";
        m_ThreatInfo[XID].Print();
        std::cout << std::endl << "O: ";
        m_ThreatInfo[OID].Print();
        std::cout << std::endl;

        std::cout << "TopThreatTier " << GetThreatInfo(XO::X).TopTier(SINGLE_THREAT)
                  << " " << GetThreatInfo(XO::O).TopTier(SINGLE_THREAT);
        std::cout << " TopDualThreatTier " << GetThreatInfo(XO::X).TopTier(DUAL_THREAT)
                  << " " << GetThreatInfo(XO::O).TopTier(DUAL_THREAT);
        std::cout << " TopFinisherTier " << GetFinisherTier(XO::X) << " " << GetFinisherTier(XO::O);
        std::cout << " TopPieceDistance " << GetPieceDistance(XO::X) << " " << GetPieceDistance(XO::O);
        std::cout << " IsTactical " << (IsTactical()?"True":"False") << std::endl;*/


    }


    int SquareInfo::Compare(SquareInfo &next, Piece own){
        Piece opposite = OppositePiece(own);   //TODO: добавить вариант равенства и учет одинарных угроз
                                                //TODO: добавить сравнение второй угрозы в случае равенства
        if(next.GetFinisherTier(own) < this->GetFinisherTier(own)
           && next.GetFinisherTier(own) <= this->GetFinisherTier(opposite)){
            return -1;
        }
        if(next.GetFinisherTier(opposite) < this->GetFinisherTier(opposite)
           && next.GetFinisherTier(opposite) < this->GetFinisherTier(own)){
            return -1;
        }
        if(this->IsTactical()){ //TODO: неверная логика, но неразрешимая без перебора
            return 1;
        }
        ThreatTier NextOwn = next.GetThreatInfo(own).TopTier(DUAL_THREAT);
        ThreatTier NextOpp = next.GetThreatInfo(opposite).TopTier(DUAL_THREAT);
        ThreatTier ThisOwn = this->GetThreatInfo(own).TopTier(DUAL_THREAT);
        ThreatTier ThisOpp = this->GetThreatInfo(opposite).TopTier(DUAL_THREAT);

        if(NextOwn < ThisOwn && NextOwn <= ThisOpp){
            return -1;
        }
        if(NextOpp < ThisOpp && NextOpp < ThisOwn){
            return -1;
        }
        if(NextOwn <= ThisOwn && NextOwn <= ThisOpp   //TODO:
            && next.GetPieceDistance(own) < this->GetPieceDistance(own)){
            return -1;
        }
        if(NextOpp <= ThisOpp && NextOpp < ThisOwn
            && next.GetPieceDistance(opposite) < this->GetPieceDistance(opposite)){
            return -1;
        }
        return 1;
    }


    int SquareInfo::GetFinisherTierSlow(Piece p){
        ThreatInfo i = GetThreatInfo(p);
        if(i.GetScore(WIN, SINGLE_THREAT) > 0){
            return WIN;
        }
        if(i.GetScore(WIN_THREAT, DUAL_THREAT) > 0){
            return WIN_THREAT;
        }
        if(i.GetScore(WIN_THREAT, SINGLE_THREAT) >= 2){
            return WIN_THREAT;
        }
        if(i.GetScore(WIN_THREAT, SINGLE_THREAT) >= 1
            && i.GetScore(PRE_WIN_THREAT, DUAL_THREAT) >= 1){
            return PRE_WIN_THREAT;
        }
        if(i.GetScore(PRE_WIN_THREAT, DUAL_THREAT) >= 2){
            return PRE_WIN_THREAT;
        }
        return NO_THREAT;

        /*int totalWTC = 0;
        int totalTODWTC = 0;
        for(auto &mask: m_masks){
            if(mask.GetSingleThreatTier(p) == WIN){
                return WIN;
            }
            int WTC = mask.ThreatCount(p, WIN_THREAT, SINGLE_THREAT);
            int TODWTC = mask.ThreatCount(p, PRE_WIN_THREAT, DUAL_THREAT);
            if(WTC > 0){
                totalWTC += WTC;
            }
            else if(TODWTC > 0){
                totalTODWTC++;
            }
        }
        if(totalWTC >= 2){
            return WIN_THREAT;
        }
        else if(totalWTC >= 1 && totalTODWTC >= 1){
            return PRE_WIN_THREAT;
        }
        else if(totalTODWTC >= 2){
            return PRE_WIN_THREAT;
        }
        else{
            return NO_THREAT;
        }*/
    }


    int SquareInfo::GetPieceDistanceSlowFromMask(PieceMask8 m, Piece p){
        int first = 4;
        for(int i = 4; i < 8; i++){
            Piece next = m.GetPiece(i);
            if(next == p){
                first = i - 4;
                break;
            }
            else if(next == OppositePiece(p) || next == OUT_OF_BOUNDS){
                first = 4;
                break;
            }
        }
        int second = 4;
        for(int i = 3; i >= 0; i--){
            Piece next = m.GetPiece(i);
            if(next == p){
                second = 3 - i;
                break;
            }
            else if(next == OppositePiece(p) || next == OUT_OF_BOUNDS){
                second = 4;
                break;
            }
        }
        return std::min(first, second);
    }


    int SquareInfo::GetPieceDistanceSlow(Piece p){
        int distance = 4;
        //TEST  временная реализация (интегрировать в сравнение клеток)
        ThreatTier tier = GetThreatInfo(p).TopTier(DUAL_THREAT);
        //TEST
        for(auto &mask: m_masks){
            if(ThreatInfo(mask, p).GetScore(tier, DUAL_THREAT)){
                distance = std::min(distance, GetPieceDistanceSlowFromMask(mask, p));
            }
        }
        return distance;
    }

    ThreatInfo SquareInfo::GetThreatInfo(Piece p){
        return m_ThreatInfo[GetPieceID(p)];
    }

    int SquareInfo::GetFinisherTier(Piece p){
        return m_FinisherTier[GetPieceID(p)];
    }


    int SquareInfo::GetPieceDistance(Piece p){
        return m_PieceDistance[GetPieceID(p)];
    }


    int SquareInfo::IsTactical(){
        return m_Tactical;
    }


    Game::Game(unsigned int w, unsigned int h):
        m_field(std::make_shared<Field>(w, h)), m_move(X), m_winner(EMPTY), m_movecount(0)
    {
        m_movelist.resize(w*h);
    }

    void Game::New(unsigned int w, unsigned int h){
        m_field->Reset(w, h);
        m_movelist.resize(w*h);
        m_move = X;
        m_winner = EMPTY;
        m_movecount = 0;
    }

    bool Game::GameOver(){
        return (Winner() != EMPTY) || !CanMove();
    }

    bool Game::XMove(){
        return m_move == X;
    }

    Piece Game::Move(){
        return m_move;
    }

    void Game::SwapMove(){
        m_move = (m_move == X) ? O : X;
    }

    Piece Game::Winner(){
        return m_winner;
    }

    int Game::GetMoveCount(){
        return m_movecount;
    }

    bool Game::CanMove(){
        return GetMoveCount() < m_field->GetSquareCount();
    }

    const Game::MoveListT& Game::GetMoveList() const{
        return m_movelist;
    }

    void Game::MakeMove(int x, int y){
        assert(Winner() == EMPTY);
        if(XMove()) {
            m_field->PlacePiece(X, x, y);
        }
        else{
            m_field->PlacePiece(O, x, y);
        }
        FieldIterator sq(m_field.get(), x, y);
        if(sq.IsWin(Move())){
            m_winner = Move();
        }
        SwapMove();
        m_movelist[m_movecount].first = x;
        m_movelist[m_movecount].second = y;
        m_movecount++;
    }

    void Game::MakeMove(const FieldIterator& pos){
        MakeMove(pos.GetX(), pos.GetY());
    }

    void Game::TakeBack(){
        assert(m_movecount >= 0);
        m_winner = EMPTY;
        m_movecount--;
        int x = m_movelist[m_movecount].first;
        int y = m_movelist[m_movecount].second;
        m_field->ClearPiece(x, y);
        SwapMove();
    }

    std::string Game::SquareToString(int x, int y){
        std::string result = std::to_string(x) + "/" + std::to_string(y) + "\n";
        result += SquareInfo(m_field.get(), x, y).ToString();
        return result;
    }

    SquareInfo Game::DumbBestMove(int &r_x, int &r_y){
        if(m_movecount == 0) {
            m_field->Middle(r_x, r_y);
            return SquareInfo(m_field.get(), r_x, r_y);
        }

        auto iter = FieldIterator(m_field.get());
        SquareInfo best = SquareInfo(m_field.get(), iter.GetX(), iter.GetY());
        r_x = iter.GetX();
        r_y = iter.GetY();
        while(++iter != OUT_OF_BOUNDS){
            if(iter.GetPiece() != EMPTY){
                continue;
            }
            SquareInfo sinfo = SquareInfo(m_field.get(), iter.GetX(), iter.GetY());
            if (best.Compare(sinfo, Move()) < 0) {
                best = sinfo;
                r_x = iter.GetX();
                r_y = iter.GetY();
            }
        }
        return best;
    };
}
