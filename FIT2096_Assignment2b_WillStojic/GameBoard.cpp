#include "GameBoard.h"
#include "MathsHelper.h"
#include <vector>

GameBoard::GameBoard()
{
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_tileShader = NULL;
}

GameBoard::GameBoard(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &tileShader, Player* &player, BulletFactory* &bulletFactory)
{
	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_tileShader = tileShader;
	m_player = player;
	m_bulletFactory = bulletFactory;

	EnemiesDefeated = false;

	Generate();
}

GameBoard::~GameBoard()
{
	for (unsigned int i = 0; i < m_monsters.size(); i++)
	{
		delete m_monsters[i];
		m_monsters[i] = NULL;
	}

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		delete m_items[i];
		m_items[i] = NULL;
	}

	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			delete m_tiles[z][x];
			m_tiles[z][x] = NULL;
		}
	}
}

void GameBoard::Update(float timestep)
{
	//tiles don't update as they are static objects

	//update items, due their animations
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		m_items[i]->Update(timestep);
	}

	//update monsters
	for (unsigned int i = 0; i < m_monsters.size(); i++)
	{
		m_monsters[i]->Update(timestep, m_player, m_bulletFactory);
	}
	
	if (m_monsters.size() <= 0)
		EnemiesDefeated = true;
}

void GameBoard::Render(Direct3D* renderer, Camera* camera)
{
	// Render all the tiles we manage

	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			m_tiles[z][x]->Render(renderer, camera);
		}
	}

	//render items
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		m_items[i]->Render(renderer, camera);
	}

	//render monsters
	for (unsigned int i = 0; i < m_monsters.size(); i++)
	{
		m_monsters[i]->Render(renderer, camera);
	}
}

void GameBoard::Generate()
{
	// Just a plain old square world for now. In the week eight lecture, we'll
	// implement an algorithm which creates cave like structures.

	for (unsigned int z = 1; z < BOARD_HEIGHT - 1; z++)
	{
		for (unsigned int x = 1; x < BOARD_WIDTH - 1; x++)
		{
			// We give a tile its mesh and shader, but it picks its own texture
			m_tiles[z][x] = new Tile(m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),
										m_tileShader,
										Vector3(x, 0, z),
										m_textureManager);

			// Spawns items onto the appropriate tiles
			if (m_tiles[z][x]->GetType() == TileType::HEALTH)
			{
				m_items.push_back(new Items(m_meshManager,
											m_tileShader,
											Vector3(x, 0, z),
											m_textureManager,
											ItemType::HEALTH));
			}
			else if (m_tiles[z][x]->GetType() == TileType::DAMAGE)
			{
				m_items.push_back(new Items(m_meshManager,
											m_tileShader,
											Vector3(x, 0, z),
											m_textureManager,
											ItemType::GEM));
			}
			else if (m_tiles[z][x]->GetType() == TileType::DISABLED)
			{
				m_items.push_back(new Items(m_meshManager,
											m_tileShader,
											Vector3(x, 0, z),
											m_textureManager,
											ItemType::AMMO));
			}
		}
	}

	AddWalls();
	SpawnMonsters();
}

void GameBoard::AddWalls()
{
	// Adds a strip of walls around the outer edge of the world

	// Outer walls horizontal
	for (unsigned int x = 0; x < BOARD_WIDTH; x++)
	{
		// Top 
		m_tiles[BOARD_HEIGHT - 1][x] = new Tile(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
			m_tileShader,
			Vector3(x, 0, BOARD_HEIGHT - 1),
			m_textureManager,
			TileType::WALL);

		// Bottom
		m_tiles[0][x] = new Tile(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
			m_tileShader,
			Vector3(x, 0, 0),
			m_textureManager,
			TileType::WALL);
	}

	// Outer walls vertical (avoding corners so we don't double up)
	for (unsigned int z = 1; z < BOARD_HEIGHT - 1; z++)
	{
		// Left
		m_tiles[z][0] = new Tile(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
			m_tileShader,
			Vector3(0, 0, z),
			m_textureManager,
			TileType::WALL);

		// Right
		m_tiles[z][BOARD_WIDTH - 1] = new Tile(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
			m_tileShader,
			Vector3(BOARD_WIDTH - 1, 0, z),
			m_textureManager,
			TileType::WALL);
	}
}

void GameBoard::SpawnMonsters()
{
	//ensures only one monster of each type is spawned.
	bool chaserSpawned = false;
	bool RetreaterSpawned = false;
	bool RandomSpawned = false;
	bool HeadOffSpawned = false;
	bool StillSpawned = false;

	while (!chaserSpawned || !RetreaterSpawned || !RandomSpawned || !HeadOffSpawned || !StillSpawned)
	{
		int randX = MathsHelper::RandomRange(1, 48);
		int randZ = MathsHelper::RandomRange(1, 48);

		if (!chaserSpawned)
		{
			m_monsters.push_back(new Monster(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
												m_tileShader,
												m_textureManager,
												Vector3(randX, 0, randZ),
												MonsterType::CHASER));
			chaserSpawned = true;
		}
		else if (!RetreaterSpawned)
		{
			m_monsters.push_back(new Monster(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
												m_tileShader,
												m_textureManager,
												Vector3(randX, 0, randZ),
												MonsterType::RETREATER));
			RetreaterSpawned = true;
		}
		else if (!RandomSpawned)
		{
			m_monsters.push_back(new Monster(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
												m_tileShader,
												m_textureManager,
												Vector3(randX, 0, randZ),
												MonsterType::RANDOM));
			RandomSpawned = true;
		}
		else if (!HeadOffSpawned)
		{
			m_monsters.push_back(new Monster(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
												m_tileShader,
												m_textureManager,
												Vector3(randX, 0, randZ),
												MonsterType::HEADOFF));
			HeadOffSpawned = true;
		}
		else if (!StillSpawned)
		{
			m_monsters.push_back(new Monster(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
												m_tileShader,
												m_textureManager,
												Vector3(randX, 0, randZ),
												MonsterType::STILL));
			StillSpawned = true;
		}
	}
}

void GameBoard::removeItem(int element)
{
	m_items.erase(m_items.begin() + element);
}

void GameBoard::removeMonster(int element)
{
	m_monsters.erase(m_monsters.begin() + element);
}

void GameBoard::DeactivateTile(int x, int z)
{
	m_tiles[z][x]->SetType(TileType::DISABLED);
}

TileType GameBoard::GetTileTypeForPosition(int x, int z)
{
	// Index directly into our 2D array using the passed in position.

	// It's possible we may accidentally check a tile outside of the board. 
	// Even though walls will prevent this, we'll still be defensive.

	if ((x < 0 || x >= BOARD_WIDTH) ||
		(z < 0 || z >= BOARD_HEIGHT))
	{
		return TileType::INVALID;
	}

	return m_tiles[z][x]->GetType();
}

Tile* GameBoard::GetRandomTileOfType(TileType type)
{
	std::vector<Tile*> shortlist;

	// Find all tiles matching the type we want
	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			if (m_tiles[z][x]->GetType() == type)
			{
				shortlist.push_back(m_tiles[z][x]);
			}
		}
	}

	// There are no more tiles left matching this type
	if (shortlist.size() == 0)
		return NULL;

	// Return a random tile from the shortlist
	return shortlist[MathsHelper::RandomRange(0, shortlist.size() - 1)];
}