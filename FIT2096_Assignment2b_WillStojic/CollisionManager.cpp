#include "CollisionManager.h"

CollisionManager::CollisionManager(Player* &player, std::vector<GameObject*>* &gameObject)
{
	m_CPlayer = player;
	m_items = gameObject;

	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions()
{
	// Check kart to item box collisions
	//PlayerToItem();

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

//void CollisionManager::PlayerToItem()
//{
//	// We'll check each kart against every item box
//	// Note this is not overly efficient, both in readability and runtime performance
//
//		for (unsigned int j = 0; j < m_itemBoxes->size(); j++)
//		{
//			// Don't need to store pointer to these objects again but favouring clarity
//			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
//			Kart* kart = (*m_karts)[i];
//			ItemBox* itemBox = (*m_itemBoxes)[j];
//
//			// TODO Fetch bounding box from the current kart and item box
//			CBoundingBox kartBounds =
//			CBoundingBox itemBoxBounds =
//
//			// TODO Are they colliding this frame?
//			bool isColliding = 
//
//			// Were they colliding last frame?
//			bool wasColliding = ArrayContainsCollision(m_previousCollisions, kart, itemBox);
//
//			if (isColliding)
//			{
//				// Register the collision
//				AddCollision(kart, itemBox);
//
//				if (wasColliding)
//				{
//					// We are colliding this frame and we were also colliding last frame - that's a collision stay
//					
//					// TODO Tell the Kart it is colliding with an Item Box (collision stay)
//					
//				}
//				else
//				{
//					// We are colliding this frame and we weren't last frame - that's a collision enter
//
//					// TODO Tell the Kart it has begun colliding with an Item Box (collision enter)
//				}
//			}
//			else
//			{
//				if (wasColliding)
//				{
//					// We aren't colliding this frame but we were last frame - that's a collision exit
//					
//					// TODO Tell the Kart it is no longer colliding with an Item Box (collision exit)
//				}
//			}
//		}
//
//}