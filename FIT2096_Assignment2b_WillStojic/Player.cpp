#include "Player.h"
#include "MathsHelper.h"

Player::Player()
{
	m_input = NULL;
	m_moveSpeed = 5.0f;
	m_currentBoard = NULL;
	m_health = 100.0f;
	m_gems = 0;
	m_ammo = 8;
	m_score = 0;
	m_monstersDefeated = 0;
}

Player::Player(Mesh * mesh, Shader * shader)
{
	m_input = NULL;
}

Player::Player(Mesh* mesh, Shader* shader, Texture* texture, InputController* input, GameBoard* board)
	: GameObject(mesh, shader, texture, Vector3::Zero)
{
	m_input = input;
	m_moveSpeed = 5.0f;
	m_currentBoard = board;
	m_health = 100.0f;
	// The player is much stronger than any monster on the board
	m_skill = 20;
	m_gems = 0;
	m_ammo = 8;
	m_score = 0;
	m_monstersDefeated = 0;

	TeleportToTileOfType(TileType::INVALID);
}

Player::~Player() {}

void Player::Update(float timestep)
{
	// Constantly step towards target position
	// Could add a distance check here, however seeing we only have one player we'll be fine
	m_position = Vector3::Lerp(m_position, m_targetPosition, timestep * m_moveSpeed);

	// We need to identify the frame input was received so we can perform common logic
	// outside of the GetKeyDown IF statements below.

	FinishTurn();

}

void Player::FinishTurn()
{
	// React to tile we're standing on
	ReactToTile();

	// Show health visually as scale of player mesh
	SetUniformScale(m_health / 100.0f);
}

bool Player::CanMoveHere(Vector3 target)
{
	// Asks the GameBoard for the type of the target tile
	// We can't step onto a wall or a disabled tile

	TileType targetTileType = m_currentBoard->GetTileTypeForPosition(target.x, target.z);

	return targetTileType != TileType::WALL;
}

void Player::ReactToTile()
{
	TileType targetTileType = m_currentBoard->GetTileTypeForPosition(m_targetPosition.x, m_targetPosition.z);

	switch (targetTileType)
	{
	case TileType::HEALTH:
		m_health += 5;
		m_currentBoard->RemoveTile(m_targetPosition);		//removes the tile from the board
		break;
	case TileType::GEM:
		m_gems += 1;
		m_currentBoard->RemoveTile(m_targetPosition);
		break;
	case TileType::AMMO:
		m_ammo += 1;
		m_currentBoard->RemoveTile(m_targetPosition);
		break;
	case TileType::MONSTER:
	{
		//gets monster on tile, and performs a battle function

		break;
	}
	default:
		break;
	}
}

void Player::MonsterDefeated()
{
	//if the player won the overall battle, increment our score and monsters, but decrease ammo by 1
	//SetScore(GetScore() + m_currentBoard->GetMonster(m_targetPosition)->GetSkill());
	SetNumberofMonstersDefeated(GetNumberOfMonstersDefeated() + 1);

	SetAmmo(GetAmmo() - 1);
	//ensure ammo doesn't enter the negatives
	if (GetAmmo() < 0)
		SetAmmo(0);

	//delete monster and their associated tile
	GetCurrentBoard()->RemoveTile(m_targetPosition);
	//GetCurrentBoard()->RemoveMonster(m_targetPosition);
}

void Player::TeleportToTileOfType(TileType type)
{
	Tile* destinationTile = m_currentBoard->GetRandomTileOfType(type);

	if (destinationTile)
	{
		// We need to set both the current position and the target
		// The only time the player remains still is when these two positions match
		m_targetPosition = destinationTile->GetPosition();
		m_position = destinationTile->GetPosition();

		// Tiles start up in the sky and fall down. Ensure player starts on the ground.
		m_targetPosition.y = 0.0f;
		m_position.y = 0.0f;
	}
}