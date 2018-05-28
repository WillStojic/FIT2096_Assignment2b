#include "BulletFactory.h"

BulletFactory::BulletFactory(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader)
{
	m_bullets;

	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_shader = shader;
}

void BulletFactory::InitialiseBullet(Vector3 &position, Vector3 &targetPosition)
{
	m_bullets.push_back(new Bullet(m_meshManager, m_textureManager, m_shader, 
									position, targetPosition));
}

void BulletFactory::InitialiseBullet(Vector3 &position, float &XRotation)
{


	//m_bullets.push_back(new Bullet(m_meshManager, m_textureManager, m_shader,
		//							position, BulletTrajectory));
}

void BulletFactory::Update(float timestep)
{
	for (int i = 0; i < m_bullets.size(); ++i)
	{

		m_bullets[i]->Update(timestep);

		if (m_bullets[i]->GetMoveSpeed() < 0)
			m_bullets.erase(m_bullets.begin() + i);
	}
}

void BulletFactory::Render(Direct3D * renderer, Camera * cam)
{
	for (int i = 0; i < m_bullets.size(); ++i)
	{
		m_bullets[i]->Render(renderer, cam);
	}
}
