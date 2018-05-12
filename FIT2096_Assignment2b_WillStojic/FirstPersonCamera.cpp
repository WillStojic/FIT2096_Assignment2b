#include "FirstPersonCamera.h"
#include "MathsHelper.h"
#include <math.h>

FirstPersonCamera::FirstPersonCamera(InputController* input)
{
	m_input = input;

	m_moveSpeed = 30.0f;
	m_rotationSpeed = 1.0f;

	m_heading = 0.0f;
	m_pitch = 0.0f;
}