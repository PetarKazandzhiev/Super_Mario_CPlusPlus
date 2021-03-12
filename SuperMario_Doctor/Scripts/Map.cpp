#include "Map.h"
#include "TextureManager.h"

//put in lvl1.txt
int lvl1[Map::MAP_SIZE_X][Map::MAP_SIZE_Y] = {
	{1, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0,-1, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 2, 2, 0, 0 ,0 ,0 ,0 ,0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,2, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 3, 0 ,0 ,0 ,0 ,0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	{1, 0, 2, 1, 1, 1 ,1 ,2 ,0 ,0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,3 ,0 ,0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0},
	{1, 2, 2, 2, 2, 2 ,2 ,2 ,2 ,2, 2, 0, 2, 2, 0, 2, 2, 0, 0, 0},
	{1, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 2, 2, 0, 0, 2, 1, 0, 0},
	{1, 4, 4, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1},
	{1, 4, 1, 1, 1, 0 ,0 ,0 ,0 ,0, 0, 0, 0, 2, 0, 1, 1, 0, 0, 1},
	{4, 4, 4, 4, 4, 0 ,0 ,0 ,0 ,1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1 ,0 ,0 ,0 ,1, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0 ,0 ,0 ,1 ,1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 3, 0, 3, 3, 3 ,3 ,1 ,1 ,1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 1 ,1 ,2 ,2 ,2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
};

Map::Map()
{
	dirt = TextureManager::loadTexture("Assets/Dirt2.png");
	grass = TextureManager::loadTexture("Assets/Wood.png");

	loadMap(lvl1);

	//which section of the sprite sheet it must take 
	rectSrc.x = 0;
	rectSrc.y = 0;
	rectSrc.h = 32;
	rectSrc.w = 32;

	//sets the size of the image in the game world
	rectDest.x = 0;
	rectDest.y = 0;
	rectDest.h = 32;
	rectDest.w = 32;
}

Map::~Map()
{
	SDL_DestroyTexture(grass);
	SDL_DestroyTexture(dirt);
}

void Map::loadMap(int arr[MAP_SIZE_X][MAP_SIZE_Y])
{
	for (int row = 0; row < MAP_SIZE_X; row++)
	{
		for (int column = 0; column < MAP_SIZE_Y; column++)
		{
			map[row][column] = arr[row][column];
			// when collisionMap[row][column] is false THERE IS collision
			if (arr[row][column] == 2 || arr[row][column] == 1) {
				collisionMap[row][column] = false;
			}
			else {
			// else there is no collision (true is no collision)
				collisionMap[row][column] = true;
			}

			// -1 => player spawn
			if (arr[row][column] == -1) {
				Map::playerSpawnX = column * 32;
				Map::playerSpawnY = row * 32;
			}
			// 3 => enemy
			else if (arr[row][column] == 3) {
				enemyMap.push_back(Vector2D(column * 32, row * 32));
			}
			// 4 => power up
			else if (arr[row][column] == 4) {

				pickupMap.push_back(Vector2D(column * 32, row * 32));
			}
			// 5 => win flag
			else if (arr[row][column] == 5) {
				Map::winX = column * 32;
				Map::winY = row * 32;
			}
		}
	}
}

void Map::drawMap()
{
	int type = 0;

	for (int row = 0; row < MAP_SIZE_X; row++)
	{
		for (int column = 0; column < MAP_SIZE_Y; column++)
		{
			type = map[row][column];

			rectDest.x = column * 32;
			rectDest.y = row * 32;

			switch (type)
			{
				case 1:
					TextureManager::draw(grass, rectSrc, rectDest);
					break;
				case 2:
					TextureManager::draw(dirt, rectSrc, rectDest);
					break;
				default:
					break;
			}
		}
	}
}

bool Map::collisionCheck(int xStart, int xEnd, int yStart, int yEnd)
{
	for (int row = 0; row < MAP_SIZE_X; ++row)
	{
		for (int column = 0; column < MAP_SIZE_Y; ++column)
		{
			if (collisionMap[row][column]) {
				continue;
			}

			int xRealCordStart = column * 32;
			int xRealCordEnd = xRealCordStart + 32;
			int yRealCordStart = row * 32;
			int yRealCordEnd = yRealCordStart + 32;

			if (((xStart > xRealCordStart && xStart < xRealCordEnd) ||
				(xEnd > xRealCordStart && xEnd < xRealCordEnd) ||
				(xStart < xRealCordStart && xEnd > xRealCordEnd) ||
				xStart == xRealCordStart || xEnd == xRealCordEnd) &&
				((yStart > yRealCordStart && yStart < yRealCordEnd) ||
					(yEnd > yRealCordStart && yEnd < yRealCordEnd) ||
					(yStart < yRealCordStart && yEnd > yRealCordEnd) ||
					yStart == yRealCordStart || yEnd == yRealCordEnd)) {
				return true;
			}
		}
	}

	return false;
}

std::vector<Vector2D> Map::getEnemyPositons() {
	return enemyMap;
}

std::vector<Vector2D> Map::getPickUpPositons() {
	return pickupMap;
}