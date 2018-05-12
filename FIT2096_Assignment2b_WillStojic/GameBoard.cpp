#include "GameBoard.h"
#include "MathsHelper.h"
#include <vector>

GameBoard::GameBoard()
{
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_tileShader = NULL;
}

GameBoard::GameBoard(MeshManager* meshManager, TextureManager* textureManager, Shader* tileShader)
{
	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_tileShader = tileShader;

	Generate();
}

GameBoard::~GameBoard()
{
	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			delete m_tiles[z][x];
			m_tiles[z][x] = NULL;
		}
	}
}

void GameBoard::Update(float timestep, GameObject* player)
{
	// Update all the tiles we manage.
	// Our tiles will have an animation so they need to be Updated each frame.

	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			if (m_tiles[z][x] != NULL)
				m_tiles[z][x]->Update(timestep);
		}
	}
}

void GameBoard::Render(Direct3D* renderer, Camera* camera)
{
	// Render all the tiles we manage

	for (unsigned int z = 0; z < BOARD_HEIGHT; z++)
	{
		for (unsigned int x = 0; x < BOARD_WIDTH; x++)
		{
			if (m_tiles[z][x] != NULL)
				m_tiles[z][x]->Render(renderer, camera);
		}
	}
}

void GameBoard::RemoveTile(Vector3 pos)
{
	delete m_tiles[(int)pos.z][(int)pos.x];
	m_tiles[(int)pos.z][(int)pos.x] = new Tile(Vector3(pos));
}

void GameBoard::Generate()
{
	// Just a plain old square world for now. In the week eight lecture, we'll
	// implement an algorithm which creates cave like structures.

	for (unsigned int z = 1; z < BOARD_HEIGHT - 1; z++)
	{
		for (unsigned int x = 1; x < BOARD_WIDTH - 1; x++)
		{
			int seed = MathsHelper::RandomRange(1,100);

			if (seed > 80)
			{
				// We give a tile its shader, but it picks its own mesh and texture
				m_tiles[z][x] = new Tile(m_meshManager,
											m_tileShader,
											Vector3(x, 0, z),
											m_textureManager);
			}
			else
				m_tiles[z][x] = new Tile(Vector3(x, 0, z));
		}
	}

	AddWalls();
}

void GameBoard::AddWalls()
{
	// Adds a strip of walls around the outer edge of the world

	// Outer walls horizontal
	for (unsigned int x = 0; x < BOARD_WIDTH; x++)
	{
		// Top 
		m_tiles[BOARD_HEIGHT-1][x] = new Tile(m_meshManager,
			m_tileShader,
			Vector3(x, 0, BOARD_HEIGHT-1),
			m_textureManager,
			TileType::WALL);

		// Bottom
		m_tiles[0][x] = new Tile(m_meshManager,
			m_tileShader,
			Vector3(x, 0, 0),
			m_textureManager,
			TileType::WALL);
	}

	// Outer walls vertical (avoding corners so we don't double up)
	for (unsigned int z = 1; z < BOARD_HEIGHT - 1; z++)
	{
		// Left
		m_tiles[z][0] = new Tile(m_meshManager,
			m_tileShader,
			Vector3(0, 0, z),
			m_textureManager,
			TileType::WALL);

		// Right
		m_tiles[z][BOARD_WIDTH-1] = new Tile(m_meshManager,
			m_tileShader,
			Vector3(BOARD_WIDTH - 1, 0, z),
			m_textureManager,
			TileType::WALL);
	}
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

	if (m_tiles[z][x] != NULL)
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
			if (m_tiles[z][x] != NULL)
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