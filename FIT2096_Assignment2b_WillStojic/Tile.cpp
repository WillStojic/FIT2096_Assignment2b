#include "Tile.h"
#include "MathsHelper.h"

Tile::Tile()
{
	m_type = TileType::INVALID;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_timeUntilMove = 0.0f;

	hasEnemy = false;
}

Tile::Tile(Vector3 pos)
{
	m_type = TileType::INVALID;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_timeUntilMove = 0.0f;
	m_position = pos;
	m_targetPosition = pos;

	hasEnemy = false;
}

Tile::Tile(MeshManager* meshManager, Shader* shader, Vector3 pos, TextureManager* textureManager)
	: GameObject(NULL, shader, NULL, pos)
{
	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_targetPosition = pos;
	m_timeUntilMove = 0.0f;

	// Also sets our texture and mesh
	SetType(SelectType());

	hasEnemy = false;

	//moves health meshes into the correct position
	if (m_type == TileType::HEALTH)
	{
		SetUniformScale(0.5f);
		SetZRotation(1.56f);
	}
	
	DropFromHeight(40.0f, 0.0f, 3.0f, MathsHelper::RandomRange(0.0f, 2.0f));
}

Tile::Tile(MeshManager* meshManager, Shader* shader, Vector3 pos, TextureManager* textureManager, TileType type)
	: GameObject(NULL, shader, NULL, pos)
{
	m_type = type;
	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_targetPosition = pos;
	m_timeUntilMove = 0.0f;

	SetType(type);

	DropFromHeight(40.0f, 0.0f, 3.0f, MathsHelper::RandomRange(0.0f, 2.0f));
}

Tile::~Tile() {}

void Tile::Update(float timestep)
{
	if (m_timeUntilMove > 0.0f)
	{
		// Not ready to fall yet
		m_timeUntilMove -= timestep;
	}
	else
	{
		// We're falling!
		m_position = Vector3::Lerp(m_position, m_targetPosition, timestep * m_moveSpeed);
	}
}

TileType Tile::SelectType()
{
	// A Tile is responsible for selecting its own type (colour) using basic random numbers
	// Higher probability for normal white tiles than the rest

	int roll = MathsHelper::RandomRange(1, 100);

	if (roll < 50)
		return TileType::MONSTER;
	else if (roll < 60)
		return TileType::HEALTH;
	else if (roll < 80)
		return TileType::AMMO;
	else
		return TileType::GEM;
}

Texture* Tile::GetTextureForType(TileType type)
{
	// Asks the texture manager for the texture matching our type (i.e. red texture for "damage" type)

	switch (type)
	{
	case TileType::HEALTH:
		return m_textureManager->GetTexture("Assets/Textures/health.png");
	case TileType::GEM:
		return m_textureManager->GetTexture("Assets/Textures/gradient_redPink.png");
	case TileType::AMMO:
		return m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png");
	default:
		return NULL;
	}
}

Mesh* Tile::GetMeshForType(TileType type)
{
	switch (type)
	{
	case TileType::HEALTH:
		return m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj");
	case TileType::GEM:
		return m_meshManager->GetMesh("Assets/Meshes/ruby.obj");
	case TileType::AMMO:
		return m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj");
	default:
		return NULL;
	}
}

void Tile::SetType(TileType type)
{
	m_type = type;
	m_texture = GetTextureForType(m_type);
	m_mesh = GetMeshForType(m_type);

	//gives random rotation to items
	if (m_type == TileType::HEALTH || m_type == TileType::AMMO || m_type == TileType::GEM)
		SetYRotation(MathsHelper::RandomRange(0.0f, 6.28f));
}

void Tile::DropFromHeight(float dropHeight, float stopHeight, float speed, float delay)
{
	// Instruct a tile to start falling from a specified height

	// Snap to drop height
	m_position.y = dropHeight;

	// Set target y to stop height (this is the resting height where the fall stops)
	m_targetPosition = Vector3(m_targetPosition.x, stopHeight, m_targetPosition.z);

	m_moveSpeed = speed;
	m_timeUntilMove = delay;
}