#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class AssetManager;
class TransformComponent;
class ColliderComponent;

class Game
{
private:
	SDL_Window* window;

	int gcd(int a, int b);
public:
	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void GameEnd(TransformComponent& playerTransformer, bool win);
	void clean();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	bool isRunning;
	static std::vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
	static AssetManager* assets;
};
