#include "./camera.hpp"

Camera::Camera()
{
	// Set Defaults
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = -3.0f;
	
	// Initialize the View Matrix
	mat4x4_identity(m_view);
	
	// Create the View Matrix
	mat4x4 view;
	vec3 eye = { m_pos[0], m_pos[1], m_pos[2] };
	vec3 center = { 0, 0, 0 };
	vec3 up = { 0, 1, 0 };
	mat4x4_look_at(view, eye, center, up);
}

void Camera::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}
