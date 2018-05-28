#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "Player.h"
#include "BulletFactory.h"
#include "GameBoard.h"

#define MAX_ALLOWED_COLLISIONS 2048

class CollisionManager
{
private:
	Player* m_Player;
	BulletFactory* m_BulletFactory;
	GameBoard* m_gameBoard;

	GameObject* m_currentCollisions[MAX_ALLOWED_COLLISIONS];

	// We need to know what objects were colliding last frame so we can determine if a collision has just begun or ended
	GameObject* m_previousCollisions[MAX_ALLOWED_COLLISIONS];

	int m_nextCurrentCollisionSlot;

	// Check if we already know about two objects colliding
	bool ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second);

	// Register that a collision has occurred
	void AddCollision(GameObject* first, GameObject* second);



	// Collision check helpers
	//first handle bullet impacts with enemies and players. Only enemy bullets can hurt the player and vice verse.
	//This stops enemies from accidentally killing eachother.
	void BulletImpactPlayer();
	void BulletImpactEnemy();
	//for when the enemy collides with the player
	void EnemyToPlayer();
	//for when the player collides with the "still" enemy's bounding sphere so that it may retreat
	void PlayerToStillSphere();
	//handles item collection and teleportation tiles
	void ItemCollect();
	void TeleportTile();

public:
	CollisionManager(Player* &player, BulletFactory* &bulletFactory, GameBoard* &gameBoard);
	void CheckCollisions();

};

#endif