#include "./camera.hpp"
#include "./vertex.hpp"
#include "./stdafx.h"

Camera::Camera()
{
	// Set Defaults
	m_dist = 2.0f;
	m_deg = 0.0f;
	m_pParent = nullptr;
	// Position
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = m_dist;
	// Forward Direction
	m_dirF[0] = 0.0f;
	m_dirF[1] = 0.0f;
	m_dirF[2] = -1.0f;
	
	// Initialize the View Matrix
	mat4x4_identity(m_v);
	
	// Set the View Matrix
	vec3 up = { 0, 1, 0 };
	vec3 center = { 0, 0, 0 };
	mat4x4_look_at(m_v, m_pos, center, up);
}

mat4x4& Camera::GetView()
{
	return m_v;
}

void Camera::SetPos(float x, float y, float z)
{
	// Set the Position
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}


void Camera::SetParent(Actor* pParent)
{
	m_pParent = pParent;
	puts("Set the Camera Parent");
}

void Camera::Orbit(float y)
{
	// Set the new Direction
	m_deg += y;
	m_dirF[0] = sin(m_deg);
	m_dirF[2] = -cos(m_deg);
}

void Camera::Update()
{
	if (m_pParent != nullptr)
	{	
		// Get the Center
		memcpy(m_center, m_pParent->GetPos(), sizeof(vec3));

		// Set the Position from the Center
		m_pos[0] = m_center[0] - m_dirF[0] * m_dist;
		m_pos[1] = m_center[1];
		m_pos[2] = m_center[2] - m_dirF[2] * m_dist;
		
		// Set the View Matrix
		mat4x4_look_at(m_v, m_pos, m_center, VEC3_UP);
	}
}
