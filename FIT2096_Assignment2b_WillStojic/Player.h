/*	FIT2096 - Assignment 1 Sample Solution
*	Player.h
*	Created by Mike Yeates - 2017 - Monash University
*	A Player listens to the keyboard and responds to arrow key presses.
*	It LERPs itself between cells and also asks the GameBoard where it can move.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "InputController.h"
#include "GameBoard.h"

class Player : public GameObject
{
protected:
	// Used to spawn the player in a random position and teleport between blue tiles
	void TeleportToTileOfType(TileType type);

private:
	// A Player should listen for its own input
	InputController* m_input;

	// We'll be animating our movement so a second position is required
	float m_moveSpeed;
	
	// Which board is the player currently on
	GameBoard* m_currentBoard;

	// Game variables
	int m_gems;
	int m_score;
	int m_monstersDefeated;
	const static int MAX_MOVES = 100;

	// Ask the GameBoard if we are allowed to move to a particular tile
	bool CanMoveHere(Vector3 target);
	
	// Housekeeping after we receive input
	void FinishTurn();

	// Check what type of tile is beneth us and react
	void ReactToTile();
	void MonsterDefeated();

public:
	Player();
	Player(Mesh* mesh, Shader* shader);
	Player(Mesh* mesh, Shader* shader, Texture* texture, InputController* input, GameBoard* board);
	~Player();

	void Update(float timestep);

	void SetCurrentBoard(GameBoard* board) { m_currentBoard = board; }
	GameBoard* GetCurrentBoard() { return m_currentBoard; }

	// Game will use these to output info to the player
	int GetNumberOfMonstersDefeated() { return m_monstersDefeated; }
	int GetScore() { return m_score; }
	int GetGems() { return m_gems; }

	void SetScore(int newScore) { m_score = newScore; }
	void SetNumberofMonstersDefeated(int monstersDefeated) { m_monstersDefeated = monstersDefeated; }
};

#endif
