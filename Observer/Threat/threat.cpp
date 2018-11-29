#include "threatcache.h"


namespace XO{
    Threat Threat::Get(Piece p, PieceSet8 mask){
        return ThreatCache::Get(p, mask);
    }
}
