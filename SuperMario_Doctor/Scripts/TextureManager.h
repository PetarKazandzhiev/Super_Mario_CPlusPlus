#pragma once
#include "Game.h"
#include "ECS/ECS.h"

class TextureManager
{
private:

public:
	static Entity* textureCamera;
	static int screenXSize;
	static int screenYSize;

	static SDL_Texture* loadTexture(const char* fileName);
	static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip = SDL_FLIP_NONE);
};