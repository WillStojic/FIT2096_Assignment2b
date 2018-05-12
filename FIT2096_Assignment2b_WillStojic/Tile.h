/*	FIT2096 - Assignment 1 Sample Solution
*	Tile.h
*	Created by Mike Yeates - 2017 - Monash University
*	A Tile represents a coloured cell on the board.
*/

#ifndef TILE_H
#define TILE_H

#include "GameObject.h"
#include "MeshManager.h"
#include "TextureManager.h"

// Define all the types of tiles we could be (naming these by function instead of appearance).
enum class TileType
{
	HEALTH,
	GEM,
	AMMO,
	MONSTER,
	WALL, //invisible wall
	INVALID // empty space
};

class Tile : public GameObject
{
private:
	TileType m_type;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	float m_timeUntilMove;
	float m_moveSpeed;

	TileType SelectType();
	Texture* GetTextureForType(TileType type);
	Mesh* GetMeshForType(TileType type);

	bool hasEnemy;
public:
	Tile();
	Tile(Vector3 pos);
	Tile(MeshManager* meshManager, Shader* shader, Vector3 pos, TextureManager* textureManager);
	Tile(MeshManager* meshManager, Shader* shader, Vector3 pos, TextureManager* textureManager, TileType type);
	~Tile();

	void Update(float timestep);

	// Instruct a tile to start falling from a specified height
	void DropFromHeight(float dropHeight, float stopHeight, float speed, float delay);

	TileType GetType() { return m_type; }
	void SetType(TileType type);

	void SetEnemyStatus(bool enemyStatus) { hasEnemy = enemyStatus; }
	bool GetEnemyStatus() { return hasEnemy; }
};

#endif

