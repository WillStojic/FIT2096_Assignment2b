#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Shader.h"

class Bullet : public GameObject 
{
private:
	float m_moveSpeed;

public:
	Bullet(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader,
			Vector3 &position, Vector3 &targetPosition);

	int GetMoveSpeed() { return m_moveSpeed; }

	void Update(float timestep);
};

#endif