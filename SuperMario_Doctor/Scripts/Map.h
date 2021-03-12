#pragma once
#include "Game.h"
#include "Vector2D.h"

class Map
{
public:
	int static const MAP_SIZE_X = 20;
	int static const MAP_SIZE_Y = 25;

	int playerSpawnX = 0;
	int playerSpawnY = 0;

	int winX = 0;
	int winY = 0;

private:
	SDL_Rect rectSrc, rectDest;
	bool collisionMap[MAP_SIZE_X][MAP_SIZE_Y];

	// list of enemy locations, loaded when the map loads
	std::vector<Vector2D> enemyMap;

	// list of pickup locations, loaded when the map loads
	std::vector<Vector2D> pickupMap;

	SDL_Texture* dirt;
	SDL_Texture* grass;

	int map[MAP_SIZE_X][MAP_SIZE_Y];
public:
	Map();
	~Map();

	void loadMap(int arr[MAP_SIZE_X][MAP_SIZE_Y]);
	void drawMap();

	bool collisionCheck(int xStart, int xEnd, int yStart, int yEnd);
	// returns list of enemy locations
	std::vector<Vector2D> getEnemyPositons();
	// returns list of pickup locations
	std::vector<Vector2D> getPickUpPositons();
};

