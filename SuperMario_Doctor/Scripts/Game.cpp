#include "Game.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

bool gameEnded = false;
bool godMode = false;
int cureCounter = 0;


std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addMovingEntity(3, 16, 21, 30));
auto& endGame(manager.addEntity());
auto& cameraEntity(manager.addEntity());
auto& poweredUp(manager.addEntity());
auto& winFlag(manager.addEntity());
auto& winGame(manager.addEntity());

Entity* TextureManager::textureCamera = &cameraEntity;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			// sets the background colour
			SDL_SetRenderDrawColor(renderer, 153, 217, 234, 255);
		}

		isRunning = true;
	}

	map = new Map();

	// camera setup
	cameraEntity.addComponent<TransformComponent>(0, 0);
	cameraEntity.getComponent<TransformComponent>().gravityAffected = false;

	// powerup fire setup
	poweredUp.addComponent<TransformComponent>(0, 0);
	poweredUp.getComponent<TransformComponent>().gravityAffected = false;
	poweredUp.addComponent<SpriteComponent>("Assets/fire.png");
	poweredUp.zIndex = 1;
	poweredUp.getComponent<SpriteComponent>().visible = false;

	//these are for the scaling of the endgame photos that appear on the screen for different screen sizes
	float gcdOfSizes = (float)gcd(TextureManager::screenXSize, 800);
	float endWinGameScale = ((float)TextureManager::screenXSize / gcdOfSizes) / (800.0f / gcdOfSizes);

	// endgame setup
	endGame.addComponent<TransformComponent>(0, 0);
	endGame.addComponent<SpriteComponent>("Assets/end_screen.png", 800, 800, endWinGameScale);
	endGame.getComponent<TransformComponent>().gravityAffected = false;
	endGame.zIndex = -1;
	endGame.getComponent<SpriteComponent>().visible = false;

	// wingame setup
	winGame.addComponent<TransformComponent>(0, 0);
	winGame.addComponent<SpriteComponent>("Assets/win_screen.png", 800, 800, endWinGameScale);
	winGame.getComponent<TransformComponent>().gravityAffected = false;
	winGame.zIndex = -1;
	winGame.getComponent<SpriteComponent>().visible = false;

	// player set up
	player.addComponent<TransformComponent>(map, map->playerSpawnX, map->playerSpawnY);
	player.addComponent<SpriteComponent>("Assets/Mario_anims.png", 32, 21, 1, true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");

	// win flag set up
	winFlag.addComponent<TransformComponent>(map, map->winX, map->winY);
	winFlag.getComponent<TransformComponent>().gravityAffected = false;
	winFlag.addComponent<SpriteComponent>("Assets/flag.png", 32, 32, 1);
	winFlag.addComponent<ColliderComponent>("winflag");

	//makes an enemy where there is a 3 on the map
	auto enemyPositions = map->getEnemyPositons();
	for (auto positon: enemyPositions) {
		auto& newGoomba(manager.addMovingEntity(2, 18, 30, 30));

		newGoomba.addComponent<TransformComponent>(map, positon.getX(), positon.getY());
		newGoomba.addComponent<SpriteComponent>("Assets/goomba.png", 32, 32, 1, true);
		newGoomba.addComponent<ColliderComponent>("enemy");
		newGoomba.addComponent<EnemyController>();
	}

	//makes an powerup where there is a 3 on the map 
	auto powerUpPositions = map->getPickUpPositons();
	for (auto positon : powerUpPositions) {
		auto& newPowerUp(manager.addEntity(30, 30));

		newPowerUp.addComponent<TransformComponent>(map, positon.getX(), positon.getY());
		newPowerUp.addComponent<SpriteComponent>("Assets/syringe.png", 32, 32);
		newPowerUp.addComponent<ColliderComponent>("powerup");
		
	}
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
	}
}

void Game::update()
{
	manager.refresh();
	manager.update();
}

//the main render function
void Game::render()
{
	SDL_RenderClear(renderer);
	
	map->drawMap();
	// calls entity draw , which call on the draw of the sprite components
	manager.draw();

	if (!gameEnded) {
		auto playerTransformer = player.getComponent<TransformComponent>();
		/*camera entity i powerup vzimat koordinatite na palyer*/
		poweredUp.getComponent<TransformComponent>().setPosition(playerTransformer.position.getX(), playerTransformer.position.getY());
		cameraEntity.getComponent<TransformComponent>().setPosition(playerTransformer.position.getX(), playerTransformer.position.getY());
		// checks for every collider in the collision list if it collides with the player 
		for (auto cc: colliders)
		{
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)) {
				// collision events 

				/*if its an enemy and the player is not in god mode
				or hasn't jumped on the enemy , the game is over*/
				if (cc->tag == "enemy") {
					if (!godMode && cc->transform->position.getY() <= playerTransformer.position.getY() + 10) {
						GameEnd(playerTransformer, false);
					}
					else {
						cc->entity->destroy();
					}
				}
				/*if the player collides with the cure the fire becomes visible
				and the player enters "god  mode" */
				else if (cc->tag == "powerup") {
					godMode = true;
					cureCounter++;
					poweredUp.getComponent<SpriteComponent>().visible = true;
					cc->entity->destroy();
				}
				//if the player collides with the winflag the winscreen appears and the game is over
				else if (cc->tag == "winflag") {
					if (godMode && cureCounter >= map->getPickUpPositons().size()) {
						GameEnd(playerTransformer, true);
					}
				}
			}
		}

		// checks if the player fell off the map
		if (playerTransformer.position.getY() >= (Map::MAP_SIZE_Y + 10) * 32) {
			GameEnd(playerTransformer, false);
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::GameEnd(TransformComponent& playerTransformer, bool win) {
	gameEnded = true;

	/*if win conditions are met the winscreen appears*/
	if (win) {
		winGame.getComponent<TransformComponent>().setPosition(playerTransformer.position.getX() - TextureManager::screenXSize / 2, playerTransformer.position.getY() - TextureManager::screenXSize / 2);
		winGame.getComponent<SpriteComponent>().visible = true;
	}
	/*else the  losescreen appears*/
	else {
		endGame.getComponent<TransformComponent>().setPosition(playerTransformer.position.getX() - TextureManager::screenXSize / 2, playerTransformer.position.getY() - TextureManager::screenXSize / 2);
		endGame.getComponent<SpriteComponent>().visible = true;
	}
	player.destroy();
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

/*namira nai-golemiqt obsht znamenatel na 2 chisla,  izpolsva se za razlichni screen golemini*/
int Game::gcd(int a, int b)
{
	int c;
	while (a != 0) {
		c = a; a = b % a;  b = c;
	}
	return b;
}