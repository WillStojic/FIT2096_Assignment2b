#include "Tile.h"
#include "MathsHelper.h"

Tile::Tile()
{
	m_type = TileType::NORMAL;
	m_textureManager = NULL;
	m_timeUntilMove = 0.0f;
}

Tile::Tile(Mesh* mesh, Shader* &shader, Vector3 pos, TextureManager* &textureManager)
	: GameObject(mesh, shader, NULL, pos)
{
	m_textureManager = textureManager;
	m_targetPosition = pos;
	m_timeUntilMove = 0.0f;

	// Also sets our texture
	SetType(SelectType());
	
}

Tile::Tile(Mesh* mesh, Shader* &shader, Vector3 pos, TextureManager* &textureManager, TileType type)
	: GameObject(mesh, shader, NULL, pos)
{
	m_type = type;
	m_textureManager = textureManager;
	m_targetPosition = pos;
	m_timeUntilMove = 0.0f;

	SetType(type);

	if (m_type == TileType::WALL)
	{
		m_scaleY = 10;
	}
}

Tile::~Tile() {}

TileType Tile::SelectType()
{
	// A Tile is responsible for selecting its own type (colour) using basic random numbers
	// Higher probability for normal white tiles than the rest

	int roll = MathsHelper::RandomRange(1, 100);

	if (roll < 95)
		return TileType::NORMAL;
	else if (roll < 96)
		return TileType::DAMAGE;
	else if (roll < 97)
		return TileType::HEALTH;
	else if (roll < 98)
		return TileType::TELEPORT;
	else
		return TileType::DISABLED;
}

Texture* Tile::GetTextureForType(TileType type)
{
	// Asks the texture manager for the texture matching our type (i.e. red texture for "damage" type)

	switch (type)
	{
	case TileType::HEALTH:
		return m_textureManager->GetTexture("Assets/Textures/tile_green.png");
	case TileType::DAMAGE:
		return m_textureManager->GetTexture("Assets/Textures/tile_red.png");
	case TileType::TELEPORT:
		return m_textureManager->GetTexture("Assets/Textures/tile_blue.png");
	case TileType::DISABLED:
		return m_textureManager->GetTexture("Assets/Textures/tile_disabled.png");
	case TileType::NORMAL:
		return m_textureManager->GetTexture("Assets/Textures/tile_white.png");
	case TileType::WALL:
		return m_textureManager->GetTexture("Assets/Textures/tile_disabled.png");
	default:
		return m_textureManager->GetTexture("Assets/Textures/tile_white.png");
	}
}

void Tile::SetType(TileType type)
{
	m_type = type;
	m_texture = GetTextureForType(m_type);

	if (type == TileType::TELEPORT)
		m_boundingBox = new CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax() + Vector3(0, 2, 0));
}