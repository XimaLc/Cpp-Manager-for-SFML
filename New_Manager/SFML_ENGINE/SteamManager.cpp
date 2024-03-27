#include "SteamManager.h"

SteamManager::SteamManager()
{
    std::cout << "------- Starting steam API load ------- \n";

    if (SteamAPI_Init())
        SteamInput()->Init(true);

    std::cout << "------- Steam API loading finish ------- \n\n\n";
}

void SteamManager::update()
{
    SteamAPI_RunCallbacks();



}

SteamManager::~SteamManager()
{
    SteamAPI_Shutdown();
}
 