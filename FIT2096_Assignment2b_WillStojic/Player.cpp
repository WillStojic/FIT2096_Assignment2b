#include "Player.h"
#include "MathsHelper.h"

Player::Player(InputController* input, FirstPersonCamera* camera)
	: PhysicsObject(Vector3::Zero)
{
	m_input = input;
	m_FPScamera = camera;
	m_health = 100.0f;
	// The player is much stronger than any monster on the board
	m_skill = 20;
	m_gems = 0;
	m_ammo = 8;
	m_score = 0;
	m_monstersDefeated = 0;

	m_moveSpeed = 0.5f;

	//player has no mesh, therefore bounds are manually set
	m_boundingBox = CBoundingBox(m_position + Vector3(-0.5f, 0.0f, -0.5f), m_position + Vector3(0.5f, 1.6f, 0.5f));
}

Player::~Player() {}

void Player::Update(float timestep)
{
	m_position = Vector3::Lerp(m_position, m_targetPosition, timestep * m_moveSpeed);

	//attaches camera to player
	m_FPScamera->SetPosition(m_position + Vector3(0.0f, 1.6f, 0.0f));


	//save on function calls by intialising local variables
	float m_heading = m_FPScamera->GetHeading();
	float m_pitch = m_FPScamera->GetPitch();

	// Accumulate change in mouse position 
	m_heading += m_input->GetMouseDeltaX() * m_FPScamera->GetRotationSpeed() * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_FPScamera->GetRotationSpeed() * timestep;

	// Limit how far the player can look down and up
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

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
	Vector3 currentPos = m_FPScamera->GetPosition();

	//moves the player's position
	if (m_input->GetKeyHold('W'))
	{
		m_targetPosition += localForwardXZ * m_moveSpeed;
	}
	if (m_input->GetKeyHold('S'))
	{
		m_targetPosition -= localForwardXZ * m_moveSpeed;
	}
	if (m_input->GetKeyHold('A'))
	{
		m_targetPosition -= localRight * m_moveSpeed;
	}
	if (m_input->GetKeyHold('D'))
	{
		m_targetPosition += localRight * m_moveSpeed;
	}

	PhysicsObject::Update(timestep);

	// Combine pitch and heading into one matrix for convenience
	Matrix lookAtRotation = pitch * heading;

	// Transform a world forward vector into local space (take pitch and heading into account)
	Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

	// At this point, our look-at vector is still relative to the origin
	// Add our position to it so it originates from the camera and points slightly in front of it
	// Remember the look-at vector needs to describe a point in the world relative to the origin
	lookAt += currentPos;
	m_rotY = lookAt.y;

	// Use parent's mutators so isDirty flags get flipped
	m_FPScamera->SetLookAt(lookAt);

	m_boundingBox.SetMin(m_position + Vector3(-0.5f, 0.0f, -0.5f));
	m_boundingBox.SetMax(m_position + Vector3(0.5f, 1.6f, 0.5f));
}