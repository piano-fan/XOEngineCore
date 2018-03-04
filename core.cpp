#include "core.h"
#include "engineinfo.h"
#include <iostream>
#include <sstream>
#include <cassert>


namespace XO{
    int IntFromSStream(std::stringstream &ss){
        if(ss.eof()){
            assert(false);
        }
        std::string word;
        ss >> word;
        return std::stoi(word);
    }

    void Core::NewGame() {
        m_game = std::make_unique<Game>();
    }

    void Core::ResetTimer() {
        m_timer_start = clock();
    }

    clock_t Core::GetTimer() {
        return clock() - m_timer_start;
    }

    Core::Core(int argc, char** argv)
    {
        ResetTimer();
        XO::Init();
        NewGame();
        std::cout << "Initialized in " << GetTimer() << "s^-6" << std::endl;
    }

    int Core::onInput(std::string input) {
        std::stringstream words(input);
        std::string command;
        words >> command;
        int code = 0;
        ResetTimer();
        if(command == "quit"){
            code = 1;
        }
        else if(command == "new"){
            NewGame();
            std::cout << "New game started in " << GetTimer() << "s^-6" << std::endl;
        }
        else if(command == "move"){
            int x, y;
            x = IntFromSStream(words);
            y = IntFromSStream(words);
            m_game->MakeMove(x, y);
        }
        else if(command == "takeback"){
            m_game->TakeBack();
        }
        else if(command == "think"){
            assert(!m_game->GameOver());
            int x,y;
            m_game->DumbBestMove(x, y);
            std::cout << "Thinked done in " << GetTimer() << "s^-6" << std::endl;
            std::cout << "move " << x << " " << y << std::endl;
        }
        else if(command == "load"){
            NewGame();
            int x, y;
            while(!words.eof()){
                x = IntFromSStream(words);
                y = IntFromSStream(words);
                m_game->MakeMove(x, y);
            }
            std::cout << "Game loaded in " << GetTimer() << "s^-6" << std::endl;
        }
        else if(command == "play"){
            int x, y;
            while(!m_game->GameOver()){
                m_game->DumbBestMove(x, y);
                m_game->MakeMove(x, y);
            }
            clock_t time = GetTimer();
            std::cout << "play ";
            const auto &movelist = m_game->GetMoveList();
            for(int i = 0; i < m_game->GetMoveCount(); i++){
                auto move = movelist[i];
                std::cout << move.first << " " << move.second << " ";
            }
            std::cout << std::endl;
            std::cout << "Game played in " << time << "s^-6" << std::endl;
        }
        else if(command == "name"){
            std::cout << "name " << XO::EngineInfo::Name() << std::endl;
        }
        else if(command == "author"){
            std::cout << "author " << XO::EngineInfo::Author() << std::endl;
        }
        else if(command == "version"){
            std::cout << "version " << XO::EngineInfo::Version() << std::endl;
        }
        else{
            std::cout << "Bad command: " << input << std::endl;
        }

        return code;
    }
}
