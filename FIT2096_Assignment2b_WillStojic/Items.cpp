#include "Items.h"
#include "MathsHelper.h"

Items::Items()
{
	m_type = ItemType::NOTHING;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_timeUntilMove = 0.0f;

	hasEnemy = false;
}

Items::Items(Vector3 pos)
{
	m_type = ItemType::NOTHING;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_timeUntilMove = 0.0f;
	m_position = pos;
	m_targetPosition = pos;

	hasEnemy = false;
}

Items::Items(MeshManager* &meshManager, Shader* &shader, Vector3 pos, TextureManager* &textureManager)
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
	if (m_type == ItemType::HEALTH)
	{
		SetUniformScale(0.5f);
		SetZRotation(1.56f);
	}

	DropFromHeight(40.0f, 0.0f, 3.0f, MathsHelper::RandomRange(0.0f, 2.0f));

	m_boundingSphere = new CBoundingSphere(m_position + Vector3(0, 1, 0), 0.3);
}

Items::Items(MeshManager* &meshManager, Shader* &shader, Vector3 pos, TextureManager* &textureManager, ItemType type)
	: GameObject(NULL, shader, NULL, pos)
{
	m_type = type;
	m_meshManager = meshManager;
	m_textureManager = textureManager;
	m_targetPosition = pos;
	m_timeUntilMove = 0.0f;

	SetType(type);

	//moves health meshes into the correct position
	if (m_type == ItemType::HEALTH)
	{
		SetUniformScale(0.5f);
		SetZRotation(1.56f);
	}

	DropFromHeight(40.0f, 0.0f, 3.0f, MathsHelper::RandomRange(0.0f, 2.0f));

	m_boundingSphere = new CBoundingSphere(m_position + Vector3(0, 1, 0), 0.3);
}

Items::~Items() {}

void Items::Update(float timestep)
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

	m_boundingSphere->SetCenter(m_position + Vector3(0, 1, 0));
	m_boundingSphere->SetRadius(0.3);
}

ItemType Items::SelectType()
{
	// A Tile is responsible for selecting its own type (colour) using basic random numbers
	// Higher probability for normal white tiles than the rest

	int roll = MathsHelper::RandomRange(1, 100);

	if (roll < 75)
		return ItemType::AMMO;
	else if (roll < 90)
		return ItemType::HEALTH;
	else
		return ItemType::GEM;
}

Texture* Items::GetTextureForType(ItemType type)
{
	// Asks the texture manager for the texture matching our type (i.e. red texture for "damage" type)

	switch (type)
	{
	case ItemType::HEALTH:
		return m_textureManager->GetTexture("Assets/Textures/health.png");
	case ItemType::GEM:
		return m_textureManager->GetTexture("Assets/Textures/gradient_redPink.png");
	case ItemType::AMMO:
		return m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png");
	default:
		return NULL;
	}
}

Mesh* Items::GetMeshForType(ItemType type)
{
	switch (type)
	{
	case ItemType::HEALTH:
		return m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj");
	case ItemType::GEM:
		return m_meshManager->GetMesh("Assets/Meshes/ruby.obj");
	case ItemType::AMMO:
		return m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj");
	default:
		return NULL;
	}
}

void Items::SetType(ItemType type)
{
	m_type = type;
	m_texture = GetTextureForType(m_type);
	m_mesh = GetMeshForType(m_type);

	//gives random rotation to items
	if (m_type == ItemType::HEALTH || m_type == ItemType::AMMO || m_type == ItemType::GEM)
		SetYRotation(MathsHelper::RandomRange(0.0f, 6.28f));
}

void Items::DropFromHeight(float dropHeight, float stopHeight, float speed, float delay)
{
	// Instruct a tile to start falling from a specified height

	// Snap to drop height
	m_position.y = dropHeight;

	// Set target y to stop height (this is the resting height where the fall stops)
	m_targetPosition = Vector3(m_targetPosition.x, stopHeight, m_targetPosition.z);

	m_moveSpeed = speed;
	m_timeUntilMove = delay;
}