#ifndef BULLET_FACTORY_H
#define BULLET_FACTORY_H

#include "Bullet.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Shader.h"
#include <vector>

class BulletFactory
{
private:
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	Shader* m_shader;

	std::vector<Bullet*> m_bullets;
public:
	BulletFactory(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader);

	void InitialiseBullet(Vector3 &position, Vector3 &targetPosition);
	void InitialiseBullet(Vector3 &position, float &XRotation);

	std::vector<Bullet*> GetBulletVector() { return m_bullets; }


	void Update(float timestep);

	void Render(Direct3D* renderer, Camera* cam);
};

#endif