#pragma once

// carries the infomration required for animation
struct Animation
{
	int index;
	int frames;
	//delay between frames
	int speed;

	Animation() {}
	Animation(int i, int f, int s)
	{
		index = i;
		frames = f;
		speed = s;
	}
};