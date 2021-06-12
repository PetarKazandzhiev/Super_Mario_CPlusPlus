#include "Scripts/Game.h"
#include "Scripts/TextureManager.h"
//#include <SDL/SDL.h>
#include "SDL.h"


Game* game = NULL;

int TextureManager::screenXSize = 600;
int TextureManager::screenYSize = screenXSize;

int main(int argc, char* argv[])
{
	const int fps = 60;
	const int frameDelay = 1000 / fps;

	Uint32 frameStart; 
	int frameTime;

	game = new Game();
	game->init("Super Doctor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TextureManager::screenXSize, TextureManager::screenYSize, false);

	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}