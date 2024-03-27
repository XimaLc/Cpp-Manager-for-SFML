
#pragma once
#include "State.h"
#include "SFML_ENGINE/SteamManager.h"


class Game
{
	WindowManager m_windowManager;
	StateStack m_state;

    SteamManager steaminit;

    void update();
    void render();
public:
	Game();
    Game(int width, int height, std::string title, bool fullscreen);
    ~Game();

    void runGame();


};

