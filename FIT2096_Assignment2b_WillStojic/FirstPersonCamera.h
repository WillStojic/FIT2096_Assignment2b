#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"
#include "InputController.h"
#include "Player.h"

class FirstPersonCamera : public Camera
{
private:
	InputController* m_input;

	float m_moveSpeed;
	float m_rotationSpeed;

	float m_heading;
	float m_pitch;

	Player* m_attachedPlayer;
public:
	FirstPersonCamera(InputController* input, Player* player);

	void Update(float timestep);
};

#endif