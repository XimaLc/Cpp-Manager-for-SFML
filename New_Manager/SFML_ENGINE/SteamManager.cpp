#include "SteamManager.h"

SteamManager::SteamManager()
{
    if (SteamAPI_Init())
        SteamInput()->Init(true);
    else
        std::cerr << "impossible load steam api";
}
 