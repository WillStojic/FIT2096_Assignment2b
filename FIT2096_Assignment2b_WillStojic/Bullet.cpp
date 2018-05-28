#include "Bullet.h"
#include <math.h>

Bullet::Bullet(MeshManager* &meshManager, TextureManager* &textureManager, Shader* &shader,
				Vector3 &position, Vector3 &targetPosition)
{
	m_mesh = meshManager->GetMesh("Assets/Meshes/bullet.obj");
	m_texture = textureManager->GetTexture("Assets/Textures/gradient_redDarker.png");
	m_shader = shader;

	m_moveSpeed = 0.7f;

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

	if (distanceToTarget <= 0.1f || m_position.y <= 0)
	{
		delete this;
	}
	else
	{
		m_boundingSphere->SetCenter(m_position);
		m_boundingSphere->SetRadius(0.1);
	}
}