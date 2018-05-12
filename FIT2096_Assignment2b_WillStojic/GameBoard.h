/*	FIT2096 - Assignment 1 Sample Solution
*	GameBoard.h
*	Created by Mike Yeates - 2017 - Monash University
*	Generates the board and manages a collection of Tiles.
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Tile.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Monster.h"
#include "GameObject.h"

class GameBoard
{
private:
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	Shader* m_tileShader;

	// How many tiles does this board manage
	const static int BOARD_WIDTH = 20;
	const static int BOARD_HEIGHT = 20;

	// Storing tiles and monsters in a 2D array to make neighbour checking easier
	Tile* m_tiles[BOARD_HEIGHT][BOARD_WIDTH];

	void Generate();
	void AddWalls();
public:
	GameBoard();
	GameBoard(MeshManager* meshManager, TextureManager* textureManager, Shader* tileShader);
	~GameBoard();

	void Update(float timestep, GameObject* player);
	void Render(Direct3D* renderer, Camera* camera);

	void RemoveTile(Vector3 pos);

	TileType GetTileTypeForPosition(int x, int z);
	Tile* GetRandomTileOfType(TileType type);

};

#endif

