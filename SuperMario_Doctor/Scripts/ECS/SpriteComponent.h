#pragma once
#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent: public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect rectSrc;
	SDL_Rect rectDest;

	bool animated = false;
	int frames = 0;
	//delay between frames
	int speed = 100;

public:
	int height = 32;
	int width = 32;
	float scale = 1;

	bool visible = true;
	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(const char* path, int h = 32, int w = 32, float sc = 1, bool isAnimated = false)
	{
		animated = isAnimated;

		height = h;
		width = w;
		scale = sc;

		Animation walk_right = Animation(0, 2, 100);
		Animation jump = Animation(1, 2, 100);

		animations.emplace("Walk", walk_right);
		animations.emplace("Jump", jump);

		Play("Walk");

		setTexture(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	//loads the texture for the sprite

	void setTexture(const char* path)
	{
		texture = TextureManager::loadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		rectSrc.x = rectSrc.y = 0;

		rectSrc.w = width;
	    rectSrc.h = height;

		rectDest.w = rectDest.h = 64;
	}

	void update() override
	{
		//if it is animated it plays the animation
		if (animated)
		{
			rectSrc.x = rectSrc.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		rectSrc.y = animIndex * height;

		//moves the rectangle where the picture is drawn
		rectDest.x = static_cast<int>(transform->getX());
		rectDest.y = static_cast<int>(transform->getY());
		rectDest.w = width * scale;
		rectDest.h = height * scale;

	}

	void draw() override
	{
		if (visible) {
			TextureManager::draw(texture, rectSrc, rectDest, spriteFlip);
		}
	}

	//plays animations (uses the array of animations used in the constructor)
	void Play(const char* animName)
	{
		animIndex = animations[animName].index;
		frames = animations[animName].frames;
		speed = animations[animName].speed;
	}
};
