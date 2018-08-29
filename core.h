#ifndef XO_CORE_H
#define XO_CORE_H

#include "XO.h"
#include <memory>


namespace XO{
    class Core{
    protected:
        std::unique_ptr<XO::Game> m_game;
    };
}


#endif //XO_CORE_H
