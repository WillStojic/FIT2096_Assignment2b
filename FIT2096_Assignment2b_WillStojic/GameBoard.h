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
#include <vector>

class GameBoard
{
private:
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	Shader* m_tileShader;

	// How many tiles does this board manage
	const static int BOARD_WIDTH = 50;
	const static int BOARD_HEIGHT = 50;

	// Storing tiles in a 2D array to make neighbour checking easier
	Tile* m_tiles[BOARD_HEIGHT][BOARD_WIDTH];

	//stores items in their own vector for easy access
	std::vector<Items*> m_items;

	void Generate();
	void AddWalls();
public:
	GameBoard();
	GameBoard(MeshManager* meshManager, TextureManager* textureManager, Shader* tileShader);
	~GameBoard();

	void Update(float timestep);
	void Render(Direct3D* renderer, Camera* camera);

	int GetBoardSize() { return BOARD_WIDTH; }
	Tile* GetTileType(int Xelement, int Yelement) { return m_tiles[Xelement][Yelement]; }
	std::vector<Items*> GetItemVector() { return m_items; }

	void removeItem(int element);

	void DeactivateTile(int x, int z);
	TileType GetTileTypeForPosition(int x, int z);
	Tile* GetRandomTileOfType(TileType type);

};

#endif

