#ifndef ITEMS_H
#define ITEMS_H

#include "GameObject.h"
#include "MeshManager.h"
#include "TextureManager.h"

// Define all the types of tiles we could be (naming these by function instead of appearance).
enum class ItemType
{
	HEALTH,
	GEM,
	AMMO,
	NOTHING
};

class Items : public GameObject
{
private:
	ItemType m_type;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	float m_timeUntilMove;
	float m_moveSpeed;

	ItemType SelectType();
	Texture* GetTextureForType(ItemType type);
	Mesh* GetMeshForType(ItemType type);

	bool hasEnemy;
public:
	Items();
	Items(Vector3 pos);
	Items(MeshManager* &meshManager, Shader* &shader, Vector3 pos, TextureManager* &textureManager);
	Items(MeshManager* &meshManager, Shader* &shader, Vector3 pos, TextureManager* &textureManager, ItemType type);
	~Items();

	void Update(float timestep);

	// Instruct a tile to start falling from a specified height
	void DropFromHeight(float dropHeight, float stopHeight, float speed, float delay);

	ItemType GetType() { return m_type; }
	void SetType(ItemType type);

	void SetEnemyStatus(bool enemyStatus) { hasEnemy = enemyStatus; }
	bool GetEnemyStatus() { return hasEnemy; }
};

#endif