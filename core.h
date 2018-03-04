#ifndef XO_CORE_H
#define XO_CORE_H

#include "XO.h"
#include <memory>
#include <string>


namespace XO{
    class Core{
        std::unique_ptr<XO::Game> m_game;
        clock_t m_timer_start;
        void NewGame();
        void ResetTimer();
        clock_t GetTimer();
    public:
        Core(int argc, char** argv);

        int onInput(std::string input);
    };
}


#endif //XO_CORE_H
