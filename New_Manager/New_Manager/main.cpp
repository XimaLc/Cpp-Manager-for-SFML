#include "SFML_ENGINE/RessourcesManager.h"
#include "Game.h"

int main()
{
	float timer = 0;
	
		/*while (1)
		{
			Tools::restartClock();
			timer += Tools::getDeltaTime();
			if(MOUSE(Left) and timer > 0.1f)
			{
				GET_MANAGER->playSound("06_FIREWORKS_Stanley_Park_Neumann_KMR_81SD_702");
				timer = 0.f;
			}
			else if (MOUSE(Right) and timer > 0.1f)
			{
				GET_MANAGER->playSound("MusicPlaceholder");
				timer = 0.f;
			}
		}*/
	



    Game game;
    game.runGame();
	return 0;
}



