/*	FIT2096 - Assignment 1 Sample Solution
*	Player.h
*	Created by Mike Yeates - 2017 - Monash University
*	A Player listens to the keyboard and responds to arrow key presses.
*	It LERPs itself between cells and also asks the GameBoard where it can move.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "PhysicsObject.h"
#include "InputController.h"
#include "FirstPersonCamera.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "BulletFactory.h"

class Player : public PhysicsObject
{
private:
	// A Player should listen for its own input
	InputController* m_input;

	Bullet* m_bulletManager;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	// Game variables
	int m_gems;
	int m_score;
	int m_monstersDefeated;
	int shootTicker;

	float m_moveSpeed;
	float m_jumpStrength;
public:
	Player(InputController* input);
	~Player();

	void Update(float timestep, FirstPersonCamera* &camera, BulletFactory* &bulletFactory);

	void TakeDamage(int damage) { m_health -= damage; }

	// Game will use these to output info to the player
	int GetNumberOfMonstersDefeated() { return m_monstersDefeated; }
	int GetScore() { return m_score; }
	int GetGems() { return m_gems; }
	float getMoveSpeed() { return m_moveSpeed; }

	void SetScore(int newScore) { m_score = newScore; }
	void SetNumberofMonstersDefeated(int monstersDefeated) { m_monstersDefeated = monstersDefeated; }
};

#endif
