#include "./camera.hpp"

Camera::Camera()
{
	// Set Defaults
	m_dist = 2.0f;
	m_rotSpeed = 0.01f;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_pParent = nullptr;
	// Position
	m_pos[0] = 0.0f;
	m_pos[1] = 1.0f;
	m_pos[2] = m_dist;
	// Forward Direction
	m_dirF[0] = 0.0f;
	m_dirF[1] = -0.77f;
	m_dirF[2] = -0.77f;
	
	// Initialize the View Matrix
	mat4x4_identity(m_v);
	
	// Compute the View Matrix
	this->ComputeView();
}

float Camera::GetYaw() const
{
	return m_yaw;
}

const vec3& Camera::GetForward() const
{	
	return m_dirF;
}

const mat4x4& Camera::GetView() const
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

void Camera::ComputeView()
{
	// If has a Parent
	if (m_pParent != nullptr)
	{	
		// Get the Center
		memcpy(m_center, m_pParent->GetPos(), sizeof(vec3));
	}
	
	// Set the Position from the Center
	m_pos[0] = m_center[0] - m_dirF[0] * m_dist;
	m_pos[1] = m_center[1] - m_dirF[1] * m_dist;
	m_pos[2] = m_center[2] - m_dirF[2] * m_dist;
	
	// Set the View Matrix
	mat4x4_look_at(m_v, m_pos, m_center, VEC3_UP);
}

void Camera::Orbit(const vec2& mDelta)
{
	// Change the Yaw and Pitch
	m_yaw += mDelta[0] * m_rotSpeed;
	m_pitch += mDelta[1] * m_rotSpeed;
	// Clamp the Pitch
	m_pitch = clampf(m_pitch, PITCH_MIN, PITCH_MAX);
	
	// Set the new Direction
	m_dirF[0] = sin(m_yaw) * cos(m_pitch);
	m_dirF[1] = sin(m_pitch);
	m_dirF[2] = -(cos(m_yaw) * cos(m_pitch));
	
	// Normalize
	vec3_norm(m_dirF, m_dirF);
}

void Camera::Update()
{
	this->ComputeView();
}
