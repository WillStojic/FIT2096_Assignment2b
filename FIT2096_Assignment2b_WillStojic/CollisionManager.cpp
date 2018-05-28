#include "CollisionManager.h"
#include "Bullet.h"
#include "Monster.h"
#include "Items.h"
#include "Tile.h"
#include "MathsHelper.h"
#include "PhysicsObject.h"

CollisionManager::CollisionManager(Player* &player, BulletFactory* &bulletFactory, GameBoard* &gameBoard)
{
	m_Player = player;
	m_BulletFactory = bulletFactory;
	m_gameBoard = gameBoard;

	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions()
{
	BulletImpactPlayer();
	BulletImpactEnemy();
	EnemyToPlayer();
	PlayerToStillSphere();
	ItemCollect();
	TeleportTile();

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

void CollisionManager::BulletImpactPlayer()
{
	for (unsigned int i = 0; i < m_BulletFactory->GetBulletVector().size(); ++i)
	{
		Bullet* bullet = m_BulletFactory->GetBulletVector().at(i);

		if (bullet->GetType() == BulletType::MONSTER)
		{
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
					m_Player->TakeDamage(MathsHelper::RandomRange(20, 35));

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
}

void CollisionManager::BulletImpactEnemy()
{
	for (unsigned int i = 0; i < m_BulletFactory->GetBulletVector().size(); ++i)
	{
		for (unsigned int j = 0; j < m_gameBoard->GetMonsterVector().size(); ++j)
		{
			Bullet* bullet = m_BulletFactory->GetBulletVector().at(i);

			if (bullet->GetType() == BulletType::PLAYER)
			{
				Monster* monster = m_gameBoard->GetMonsterVector().at(j);

				CBoundingSphere* bulletBounds = bullet->GetBoundingSphere();
				CBoundingBox* enemyBounds = monster->GetBoundingBox();

				bool isColliding = CheckCollision(*bulletBounds, *enemyBounds);

				bool wasColliding = ArrayContainsCollision(m_previousCollisions, bullet, monster);

				if (isColliding)
				{
					AddCollision(bullet, monster);

					if (wasColliding)
					{
						//collision stay

					}
					else
					{
						//collision enter
						monster->TakeDamage(MathsHelper::RandomRange(30, 50));

						//deletes monster if dead and increases player's score
						if (monster->GetHealth() <= 0)
						{
							m_gameBoard->removeMonster(j);
							delete monster;
							m_Player->SetScore(m_Player->GetScore() + 100);
							m_Player->IncrementNumberofMonstersDefeated();
						}
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
	}
}

void CollisionManager::EnemyToPlayer()
{
	for (unsigned int i = 0; i < m_gameBoard->GetMonsterVector().size(); ++i)
	{
		Monster* monster = m_gameBoard->GetMonsterVector().at(i);

		CBoundingBox* enemyBounds = monster->GetBoundingBox();
		CBoundingBox* playerBounds = m_Player->GetBoundingBox();

		bool isColliding = CheckCollision(*enemyBounds, *playerBounds);

		bool wasColliding = ArrayContainsCollision(m_previousCollisions, monster, m_Player);

		if (isColliding)
		{
			AddCollision(monster, m_Player);

			if (wasColliding)
			{
				//collision stay

			}
			else
			{
				//collision enter
				m_Player->TakeDamage(1000);
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

void CollisionManager::PlayerToStillSphere()
{
	for (unsigned int i = 0; i < m_gameBoard->GetMonsterVector().size(); ++i)
	{
		Monster* monster = m_gameBoard->GetMonsterVector().at(i);

		if (monster->GetType() == MonsterType::STILL)
		{
			CBoundingSphere* stillBounds = monster->GetBoundingSphere();
			CBoundingBox* playerBounds = m_Player->GetBoundingBox();

			bool isColliding = CheckCollision(*stillBounds, *playerBounds);

			bool wasColliding = ArrayContainsCollision(m_previousCollisions, monster, m_Player);

			if (isColliding)
			{
				AddCollision(monster, m_Player);

				if (wasColliding)
				{
					//collision stay
					monster->Retreat(m_Player);
				}
				else
				{
					//collision enter
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
}

void CollisionManager::ItemCollect()
{
	for (unsigned int i = 0; i < m_gameBoard->GetItemVector().size(); ++i)
	{
		Items* item = m_gameBoard->GetItemVector().at(i);

		CBoundingSphere* itemBounds = item->GetBoundingSphere();
		CBoundingBox* playerBounds = m_Player->GetBoundingBox();

		bool isColliding = CheckCollision(*itemBounds, *playerBounds);

		bool wasColliding = ArrayContainsCollision(m_previousCollisions, item, m_Player);

		if (isColliding)
		{
			AddCollision(item, m_Player);

			if (wasColliding)
			{
				//collision stay

			}
			else
			{
				//collision enter
				m_Player->PickupItem(item->GetType());
				delete item;
				m_gameBoard->removeItem(i);
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

void CollisionManager::TeleportTile()
{
	for (unsigned int i = 0; i < m_gameBoard->GetBoardSize(); ++i)
	{
		for (unsigned int j = 0; j < m_gameBoard->GetBoardSize(); ++j)
		{
			Tile* tile = m_gameBoard->GetTileType(i, j);

			if (tile->GetType() == TileType::TELEPORT)
			{

				CBoundingBox* tileBounds = tile->GetBoundingBox();
				CBoundingBox* playerBounds = m_Player->GetBoundingBox();

				bool isColliding = CheckCollision(*tileBounds, *playerBounds);

				bool wasColliding = ArrayContainsCollision(m_previousCollisions, tile, m_Player);

				if (isColliding)
				{
					AddCollision(tile, m_Player);

					if (wasColliding)
					{
						//collision stay

					}
					else
					{
						//collision enter
						m_Player->SetPosition(m_gameBoard->GetRandomTileOfType(TileType::TELEPORT)->GetPosition());
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
	}
}

