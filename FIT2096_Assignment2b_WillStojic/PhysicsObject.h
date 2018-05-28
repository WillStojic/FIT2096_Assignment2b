#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "GameObject.h"

class PhysicsObject : public GameObject
{
protected:
	// Implementing basic physics
	// http://natureofcode.com/book/chapter-2-forces/

	Vector3 m_velocity;
	Vector3 m_acceleration;
	
	Vector3 m_positionDelta;
	Vector3 m_gravity = Vector3(0.0f, -0.01f, 0.0f);

	float m_frictionAmount;

	void ApplyForce(Vector3 force);
	void ApplyFriction(float strength);
	void Jump(float strength);

public:
	PhysicsObject(Vector3 position);
	PhysicsObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

	virtual void Update(float timestep);
};

#endif