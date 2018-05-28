#include "PhysicsObject.h"

void PhysicsObject::ApplyForce(Vector3 force)
{
	// This is force accumulation in action - all the forces
	// we want to apply add together and are
	// stored in acceleration
	m_acceleration += force;
}

void PhysicsObject::ApplyFriction(float strength)
{
	// Weak force in opposite direction to velocity
	ApplyForce(-m_velocity * strength);
}

void PhysicsObject::Jump(float strength)
{
	m_positionDelta = Vector3(0, 0, 0);
	m_positionDelta += Vector3(0.0f, strength, 0.0f);
}

PhysicsObject::PhysicsObject(Vector3 position)
{
	m_mesh = NULL;
	m_shader = NULL;
	m_texture = NULL;
	m_position = position;
	m_positionDelta = Vector3(0.0f, 0.0f, 0.0f);
}

PhysicsObject::PhysicsObject(Mesh * mesh, Shader * shader, Texture * texture, Vector3 position)
	: GameObject(mesh, shader, texture, position)
{
	m_mesh = mesh;
	m_shader = shader;
	m_texture = texture;
	m_position = position;

	m_acceleration = Vector3::Zero;
	m_velocity = Vector3::Zero;

	m_frictionAmount = 10.0f;
}

void PhysicsObject::Update(float timestep)
{
	// Apply friction every frame 
	ApplyFriction(m_frictionAmount);

	// Acceleration trickles down into velocity
	m_velocity += m_acceleration;

	// Velocity trickles down into position
	m_position += m_velocity;

	m_positionDelta += m_gravity;
	m_position += m_positionDelta;

	if (m_position.y < 0)
		m_position.y = 0;

	// Reset acceleration each frame so it doesn't accumulate
	// beyond a single frame. Only velocity lasts between frames
	m_acceleration = Vector3::Zero;
}
