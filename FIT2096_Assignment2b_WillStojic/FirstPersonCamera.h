#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"
#include "InputController.h"

class FirstPersonCamera : public Camera
{
private:
	InputController* m_input;

	float m_moveSpeed;
	float m_rotationSpeed;

	float m_heading;
	float m_pitch;
public:
	FirstPersonCamera(InputController* input);

	float GetHeading() { return m_heading; }
	float GetPitch() { return m_pitch; }
	float GetRotationSpeed() const { return m_rotationSpeed; }

	void SetHeading(float heading) { m_heading = heading; }
	void SetPitch(float pitch) { m_pitch = pitch; }
};

#endif