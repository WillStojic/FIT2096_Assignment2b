#include "CollisionManager.h"
#include "Bullet.h"
#include "MathsHelper.h"

CollisionManager::CollisionManager(Player* &player, BulletFactory* &bulletFactory)
{
	m_Player = player;
	m_BulletFactory = bulletFactory;

	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions()
{
	BulletImpact();

	// Move all current collisions into previous
	memcpy(m_previousCollisions, m_currentCollisions, sizeof(m_currentCollisions));

	// Clear out current collisions
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	
	// Now current collisions is empty, we'll start adding from the start again
	m_nextCurrentCollisionSlot = 0;

}

bool CollisionManager::ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second)
{
	// See if these two GameObjects appear one after the other in specified collisions array
	// Stop one before length so we don't overrun as we'll be checking two elements per iteration
	for (int i = 0; i < MAX_ALLOWED_COLLISIONS - 1; i += 2)
	{
		if ((arrayToSearch[i] == first && arrayToSearch[i + 1] == second) ||
			arrayToSearch[i] == second && arrayToSearch[i + 1] == first)
		{
			// Found them!
			return true;
		}
	}

	// These objects were not colliding last frame
	return false;
}

void CollisionManager::AddCollision(GameObject* first, GameObject* second)
{	
	// Add the two colliding objects to the current collisions array
	// We keep track of the next free slot so no searching is required
	m_currentCollisions[m_nextCurrentCollisionSlot] = first;
	m_currentCollisions[m_nextCurrentCollisionSlot + 1] = second;
	
	m_nextCurrentCollisionSlot += 2;
}

void CollisionManager::BulletImpact()
{
	for (unsigned int i = 0; i < m_BulletFactory->m_bullets.size(); ++i)
	{
		Bullet* bullet = m_BulletFactory->m_bullets[i];

		CBoundingSphere* bulletBounds = bullet->GetBoundingSphere();
		CBoundingBox* playerBounds = m_Player->GetBoundingBox();

		bool isColliding = CheckCollision(*bulletBounds, *playerBounds);

		bool wasColliding = ArrayContainsCollision(m_previousCollisions, bullet, m_Player);

		if (isColliding)
		{
			AddCollision(bullet, m_Player);

			if (wasColliding)
			{
				//collision stay

			}
			else
			{
				//collision enter
				m_Player->TakeDamage(MathsHelper::RandomRange(10, 30));
			}
		}
		else
		{
			if (wasColliding)
			{
				//collision exit

			}
		}
	}
}