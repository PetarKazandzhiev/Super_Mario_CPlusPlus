#include "TextureManager.h"
#include "ECS/TransformComponent.h"

SDL_Texture* TextureManager::loadTexture(const char* fileName)
{
	//loads an image and makes it a surfice
	SDL_Surface* tempSurface = IMG_Load(fileName);
	//makes a texture from the surfice
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	//takes care of the camera
	dest.x -= textureCamera->getComponent<TransformComponent>().position.getX() - screenXSize / 2 ;
	dest.y -= textureCamera->getComponent<TransformComponent>().position.getY() - screenYSize / 2 ;

	//SDL_RenderCopy doesn't allow us to rotate, flip the texture
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}