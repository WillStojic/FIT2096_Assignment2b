#include "Monster.h"
#include "MathsHelper.h"

Monster::Monster()
	: PhysicsObject(Vector3::Zero)
{
	SetHealth(100);
}

Monster::Monster(Mesh* mesh, Shader* &shader, TextureManager* &textureManager, Vector3 position, MonsterType type)
	: PhysicsObject(position)
{
	m_mesh = mesh;
	m_shader = shader;
	m_textureManager = textureManager;
	m_position = position;
	m_targetPosition = position;
	m_type = type;

	//firerate is random for every enemy.
	m_fireRate = MathsHelper::RandomRange(50, 150);
	
	//sets random health
	SetHealth(MathsHelper::RandomRange(50.0f, 150.0f));

	//sets texture, corresponds to health level, and also changes move speed.
	m_texture = GetTextureForVar();

	//creates bounding box
	m_boundingBox = new CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());

	//creates a bounding sphere for the "STILL" monster class. Causes the mosnter to retreat when player is inside.
	if (m_type == MonsterType::STILL)
		m_boundingSphere = new CBoundingSphere(m_position, 6);
}

Monster::~Monster() {}

void Monster::Retreat(Player* &player)
{
	//calculates direction vector
	Vector3 enemyToUs = m_position - player->GetPosition();
	enemyToUs.Normalize();

	//moves opposite to player position, has a tendency to get stuck in corners, which would ruin their bounds boxes.
	//thus, enemy is repelled from walls, so that bullets will collide with them again.

	if (m_position.x >= 48)
		m_targetPosition += Vector3(-5, 0, 0) * m_moveSpeed;
	else if (m_position.z >= 48)
		m_targetPosition += Vector3(0, 0, -5) * m_moveSpeed;
	else if (m_position.x <= 1)
		m_targetPosition += Vector3(0, 0, 5) * m_moveSpeed;
	else if (m_position.z <= 1)
		m_targetPosition += Vector3(5, 0, 0) * m_moveSpeed;
	else
		m_targetPosition += enemyToUs * m_moveSpeed;
}

void Monster::Update(float timestep)
{
}

void Monster::Update(float timestep, Player* &player, BulletFactory* &bulletFactory)
{
	m_position = Vector3::Lerp(m_position, m_targetPosition, timestep * m_moveSpeed);

	Vector3 playerDir;

	playerDir.x = player->GetPosition().x - m_position.x;
	playerDir.z = player->GetPosition().z - m_position.z;

	m_rotY = atan2(playerDir.x, playerDir.z);		//rotates to player pos



	//shoots at the player after a random amount of frames.
	//enemies cannot shoot on the Y access. This lets the player predict bullet paths easily, and lets you dodge more reliably by jumping.
	++m_shootTicker;
	if (m_shootTicker > m_fireRate && player->GetHealth() > 0)
	{
		Vector3 offset = m_position + Vector3(0, 1.4, 0);

		Vector3 target = playerDir * 100;

		bulletFactory->InitialiseBullet(offset, target, BulletType::MONSTER);

		m_shootTicker = 0;
	}




	//logic for all 5 types of monsters. 
	//NOTE: the "still" monster type has no function in Update(). Instead, it has a collision sphere which calls the Retreat()
	//function through CollisionManager when the player enters its radius.
	++m_chaserEngageTicker;
	if (m_type == MonsterType::CHASER && m_chaserEngageTicker > 300) 
	{
		//follows player
		m_targetPosition = player->GetPosition();
	}
	else if (m_type == MonsterType::RETREATER)
	{
		Retreat(player);
	}
	else if (m_type == MonsterType::RANDOM)
	{
		//due to LERP, a new point to chosen once m_position is almost at m_targetPosition.
		if (Vector3::Distance(m_position, m_targetPosition) < 0.5f)
			m_targetPosition = Vector3(MathsHelper::RandomRange(1, 48), 0, MathsHelper::RandomRange(1, 48));
		//random range is used to generate a random point on the board's X and Z axis.
	}
	else if (m_type == MonsterType::HEADOFF)
	{
		//the player has a Vector3 variable which contains a point directly in front of the player, generated from the camera lookAt matrix.
		m_targetPosition = player->GetHeadOff();
	}




	//prevents the monster from floating above the board if the player jumps
	if (m_position.y > 0)
		m_position.y = 0;

	//updates the collision sphere for the "still" monster
	if (m_type == MonsterType::STILL)
	{
		m_boundingSphere->SetCenter(m_position);
		m_boundingSphere->SetRadius(6);
	}

	m_boundingBox->SetMin(m_position + m_mesh->GetMin());
	m_boundingBox->SetMax(m_position + m_mesh->GetMax());

	PhysicsObject::Update(timestep);
}

Texture* Monster::GetTextureForVar()
{
	//gets different texture, based on health of the monster, move speed is also reduced has health increases.
	if (m_health < 60)
	{
		m_moveSpeed = 2.0;
		return m_textureManager->GetTexture("Assets/Textures/gradient_redPink.png");
	}
	else if (m_health < 80)
	{
		m_moveSpeed = 1.5;
		return m_textureManager->GetTexture("Assets/Textures/gradient_redLighter.png");
	}
	else if (m_health < 100)
	{
		m_moveSpeed = 1.3;
		return m_textureManager->GetTexture("Assets/Textures/gradient_redOrange.png.png");
	}
	else if (m_health < 120)
	{
		m_moveSpeed = 0.9;
		return m_textureManager->GetTexture("Assets/Textures/gradient_red.png");
	}
	else
	{
		m_moveSpeed = 0.5;
		return m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png");
	}
}