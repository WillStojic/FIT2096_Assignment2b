#include "BulletFactory.h"

BulletFactory::BulletFactory(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader)
{
	m_bullets;

	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_shader = shader;
}

BulletFactory::~BulletFactory()
{
	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		delete m_bullets[i];
		m_bullets[i] = NULL;
	}
}

void BulletFactory::InitialiseBullet(Vector3 &position, Vector3 &targetPosition, BulletType type)
{
	m_bullets.push_back(new Bullet(m_meshManager, m_textureManager, m_shader, 
									position, targetPosition, type));
}

void BulletFactory::RemoveBullet(int element)
{
	m_bullets.erase(m_bullets.erase(m_bullets.begin() + element));
}

void BulletFactory::Update(float timestep)
{
	for (int i = 0; i < m_bullets.size(); ++i)
	{

		m_bullets[i]->Update(timestep);

		//automatically removes bullets if they are at a nonesense value.
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
