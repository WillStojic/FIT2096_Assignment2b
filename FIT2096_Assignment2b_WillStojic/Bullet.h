#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Shader.h"

enum BulletType
{
	PLAYER,
	MONSTER
};

class Bullet : public GameObject 
{
private:
	float m_moveSpeed;
	BulletType m_type;
public:
	Bullet(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader,
			Vector3 &position, Vector3 &targetPosition, BulletType type);

	int GetMoveSpeed() { return m_moveSpeed; }
	BulletType GetType() { return m_type; }

	void Update(float timestep);
};

#endif