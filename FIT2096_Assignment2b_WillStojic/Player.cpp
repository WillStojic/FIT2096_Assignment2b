#include "Player.h"
#include "MathsHelper.h"

Player::Player(InputController* input)
	: PhysicsObject(Vector3(MathsHelper::RandomRange(1, 48), 0, MathsHelper::RandomRange(1, 48)))
{
	m_input = input;
	// The player is much stronger than any monster on the board
	m_health = 200.0f;
	m_gems = 0;
	//starting ammo
	m_ammo = 8;
	m_score = 0;
	m_monstersDefeated = 0;

	//amount of frames before the player can fire another bullet.
	m_fireRate = 15;
	//movement attributes
	m_moveSpeed = 5.0f;
	m_jumpStrength = 0.3f;

	//player has no mesh, therefore bounds are manually set, mesh is kept relatively thin, so the player can dodge bullets more easily
	m_boundingBox = new CBoundingBox(m_position + Vector3(-0.15f, 0.0f, -0.15f), m_position + Vector3(0.15f, 1.8f, 0.15f));
}

Player::~Player() {}

void Player::Update(float timestep, FirstPersonCamera* &camera, BulletFactory* &bulletFactory)
{
	m_position = Vector3::Lerp(m_position, m_targetPosition, timestep * m_moveSpeed);
	//disables player input, if health is below 0
	if (m_health <= 0)
	{
		//player falls on their back when dead.
		camera->SetPosition(m_position + Vector3(0.0f, 0.3f, 0.0f));
	}
	else
	{
		//attaches camera to player
		camera->SetPosition(m_position + Vector3(0.0f, 1.6f, 0.0f));

		//save on function calls by intialising local variables
		float m_heading = camera->GetHeading();
		float m_pitch = camera->GetPitch();

		// Accumulate change in mouse position 
		m_heading += m_input->GetMouseDeltaX() * camera->GetRotationSpeed() * timestep;
		m_pitch += m_input->GetMouseDeltaY() * camera->GetRotationSpeed() * timestep;


		// Limit how far the player can look down and up
		m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

		camera->SetHeading(m_heading);
		camera->SetPitch(m_pitch);

		// Wrap heading and pitch up in a matrix so we can transform our look at vector
		// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
		// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
		Matrix heading = Matrix::CreateRotationY(m_heading);
		Matrix pitch = Matrix::CreateRotationX(m_pitch);

		// Transform a world right vector from world space into local space
		Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), heading);

		// Essentially our local forward vector but always parallel with the ground
		// Remember a cross product gives us a vector perpendicular to the two input vectors
		Vector3 localForwardXZ = localRight.Cross(Vector3(0, 1, 0));

		// We're going to need this a lot. Store it locally here to save on our function calls 
		Vector3 currentPos = camera->GetPosition();

		Vector3 translation = m_position;

		//moves the player's position
		if (m_input->GetKeyHold('W'))
		{
			translation += localForwardXZ;
		}
		if (m_input->GetKeyHold('S'))
		{
			translation -= localForwardXZ;
		}
		if (m_input->GetKeyHold('A'))
		{
			translation -= localRight;
		}
		if (m_input->GetKeyHold('D'))
		{
			translation += localRight;
		}
		if (m_input->GetKeyHold(VK_SHIFT))
		{
			m_moveSpeed = 10;
		}
		else
			m_moveSpeed = 5;
		if (m_input->GetKeyDown(VK_SPACE) && m_position.y < 0.1)
		{
			this->Jump(m_jumpStrength);
		}

		m_targetPosition = translation;

		// Combine pitch and heading into one matrix for convenience
		Matrix lookAtRotation = pitch * heading;

		// Transform a world forward vector into local space (take pitch and heading into account)
		Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

		m_headOffPoint = (lookAt * 10) + currentPos;

		//handles shooting input
		++m_shootTicker;
		if (m_input->GetMouseDown(0) && m_shootTicker > m_fireRate && m_ammo != 0)
		{
			//creates bullet trajectory
			Vector3 aim = lookAt * 50;
			aim += currentPos;
			Vector3 offset = Vector3::TransformNormal(Vector3(0, 0, 0.5), lookAtRotation) + currentPos;

			//spawns bullet
			bulletFactory->InitialiseBullet(offset, aim, BulletType::PLAYER);

			//reduces ammo after each shot
			--m_ammo;

			//resets shoot ticker, thus meaning the player will have to wait a set amount of frames to shoot again
			m_shootTicker = 0;
		}

		// At this point, our look-at vector is still relative to the origin
		// Add our position to it so it originates from the camera and points slightly in front of it
		// Remember the look-at vector needs to describe a point in the world relative to the origin
		lookAt += currentPos;

		//orient player's y rotation to camera
		m_rotY = lookAt.y;

		// Use parent's mutators so isDirty flags get flipped
		camera->SetLookAt(lookAt);
		camera->SetPosition(currentPos);

		m_boundingBox->SetMin(m_position + Vector3(-0.05f, 1.1f, -0.05f));
		m_boundingBox->SetMax(m_position + Vector3(0.05f, 1.8f, 0.05f));

		PhysicsObject::Update(timestep);
	}
}


//simple adjustment to player variable once item is collided with.
void Player::PickupItem(ItemType itemType)
{
	if (itemType == ItemType::HEALTH)
		m_health += 10;
	else if (itemType == ItemType::AMMO)
		m_ammo += 4;
	else if (itemType == ItemType::GEM)
	{
		++m_gems;
		m_score += 40;
	}
}
