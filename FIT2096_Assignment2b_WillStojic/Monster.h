#ifndef MONSTER_H
#define MONSTER_H

#include "PhysicsObject.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "BulletFactory.h"
#include "Player.h"
#include <vector>

//all different types of monster
enum MonsterType
{
	CHASER,
	RETREATER,
	RANDOM,
	HEADOFF,
	STILL
};

class Monster : public PhysicsObject
{
private:
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	MonsterType m_type;

	float m_moveSpeed;

	//how many frames until another shot can be fired
	int m_shootTicker;
	int m_fireRate;
	//how long before the Chaser will start following the player, allows you to get you bearings before he comes after you
	int m_chaserEngageTicker;

	Texture* GetTextureForVar();
public:
	Monster();
	Monster(Mesh* mesh, Shader* &shader, TextureManager* &textureManager, Vector3 position, MonsterType type);
	~Monster();

	//take damage from bullet
	void TakeDamage(int damage) { m_health -= damage; }

	//for still enemy, once play is within sphere radius
	void Retreat(Player* &player);

	MonsterType GetType() { return m_type; }

	void Update(float timestep);
	void Update(float timestep, Player* &player, BulletFactory* &bulletFactory);
};

#endif