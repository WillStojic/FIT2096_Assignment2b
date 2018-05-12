#include "Monster.h"
#include "MathsHelper.h"

Monster::Monster()
{
	SetHealth(100);
	m_skill = MathsHelper::RandomRange(3, 10);
}

Monster::Monster(Mesh* mesh, Shader* shader, TextureManager* textureManager, Vector3 position)
	: GameObject(mesh, shader, NULL, position)
{
	m_mesh = mesh;
	m_shader = shader;
	m_textureManager = textureManager;
	m_position = position;
	m_targetPosition = position;

	SetHealth(MathsHelper::RandomRange(50.0f, 150.0f));
	m_skill = MathsHelper::RandomRange(3, 10);
	m_ammo = 1;

	m_texture = GetTextureForVar();
}

Monster::~Monster() {}

void Monster::Update(float timestep)
{
}

void Monster::Update(float timestep, GameObject* player)
{
	Vector3 playerDir;

	playerDir.x = player->GetPosition().x - m_position.x;
	playerDir.z = player->GetPosition().z - m_position.z;

	m_rotY = atan2(playerDir.x, playerDir.z);		//rotates to player pos
}

Texture* Monster::GetTextureForVar()
{
	//gets different texture, based on skill level of the monster
	if (m_skill < 8)
		return m_textureManager->GetTexture("Assets/Textures/gradient_redPink.png");
	else if (m_skill < 6)
		return m_textureManager->GetTexture("Assets/Textures/gradient_redLighter.png");
	else if (m_skill < 4)
		return m_textureManager->GetTexture("Assets/Textures/tile_orange.png");
	else if (m_skill < 2)
		return m_textureManager->GetTexture("Assets/Textures/gradient_red.png");
	else
		return m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png");
}