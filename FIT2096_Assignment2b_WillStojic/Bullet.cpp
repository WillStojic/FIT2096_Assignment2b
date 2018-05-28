#include "Bullet.h"
#include <math.h>

Bullet::Bullet(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader,
				Vector3 &position, Vector3 &targetPosition, BulletType type)
{
	m_mesh = meshManager->GetMesh("Assets/Meshes/bullet.obj");
	m_texture = textureManager->GetTexture("Assets/Textures/gradient_redLighter.png");
	m_shader = shader;
	m_type = type;

	m_moveSpeed = 0.3f;

	m_position = position;
	m_targetPosition = targetPosition;

	m_rotY = atan2(m_targetPosition.x - m_position.x, m_targetPosition.z - m_position.z);

	m_boundingSphere = new CBoundingSphere(m_position, 0.1);
}

void Bullet::Update(float timestep)
{
	Vector3 toTarget = m_targetPosition - m_position;
	toTarget.Normalize();

	m_position += toTarget * m_moveSpeed;

	float distanceToTarget = Vector3::Distance(m_position, m_targetPosition);

	//deletes bullet if its out of the play area, otherwise, bounds are updated
	if (distanceToTarget <= 0.5f || m_position.y <= 0 || m_position.x > 49 || m_position.x < 0 || m_position.z > 49 || m_position.z < 0)
	{
		delete this;
	}
	else
	{
		m_boundingSphere->SetCenter(m_position);
		m_boundingSphere->SetRadius(0.1);
	}
}