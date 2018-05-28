/*	FIT2096 - Assignment 1 Sample Solution
*	GameBoard.h
*	Created by Mike Yeates - 2017 - Monash University
*	Generates the board and manages a collection of Tiles.
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Tile.h"
#include "Items.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Collisions.h"
#include "Monster.h"
#include "Player.h"
#include "BulletFactory.h"
#include <vector>

class GameBoard
{
private:
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	Shader* m_tileShader;
	Player* m_player;
	BulletFactory* m_bulletFactory;

	// How many tiles does this board manage
	const static int BOARD_WIDTH = 50;
	const static int BOARD_HEIGHT = 50;

	// Storing tiles in a 2D array to make neighbour checking easier
	Tile* m_tiles[BOARD_HEIGHT][BOARD_WIDTH];

	//stores items and monsters in their own vectors for easy access
	std::vector<Items*> m_items;
	std::vector<Monster*> m_monsters;

	bool EnemiesDefeated;

	//generate adds tiles and items onto the corresponding tile
	void Generate();
	void AddWalls();

	//only five monsters are spawned, randomly into the arena. One of each type is spawned.
	void SpawnMonsters();
public:
	GameBoard();
	GameBoard(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &tileShader, Player* &player, BulletFactory* &bulletFactory);
	~GameBoard();

	void Update(float timestep);
	void Render(Direct3D* renderer, Camera* camera);


	//returns tiles
	int GetBoardSize() { return BOARD_WIDTH; }
	Tile* GetTileType(int Xelement, int Yelement) { return m_tiles[Xelement][Yelement]; }

	//gets vecters of monsters and items
	std::vector<Items*> GetItemVector() { return m_items; }
	std::vector<Monster*> GetMonsterVector() { return m_monsters; }

	bool GetEnemiesDefeated() { return EnemiesDefeated; }

	void removeItem(int element);
	void removeMonster(int element);

	void DeactivateTile(int x, int z);
	TileType GetTileTypeForPosition(int x, int z);
	Tile* GetRandomTileOfType(TileType type);

};

#endif

