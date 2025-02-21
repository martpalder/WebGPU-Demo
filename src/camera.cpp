#include "./camera.hpp"

Camera::Camera()
{
	// Set Defaults
	m_dist = 2.0f;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_pParent = nullptr;
	
	// Position
	this->SetPos(0.0f, 0.0f, -m_dist);
	// Direction
	this->SetDir(0.0f, 0.0f, 1.0f);
	
	// Initialize the Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_v);
	
	// Set the View Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	mat4x4_rotate_X(m_r, m_r, 1.0f);
	mat4x4_mul(m_v, m_t, m_r);
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
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

void Camera::SetDir(float x, float y, float z)
{
	m_dirF[0] = x;
	m_dirF[1] = y;
	m_dirF[2] = z;
}

void Camera::SetParent(Actor* pParent)
{
	m_pParent = pParent;
	puts("Set the Camera Parent");
}

void Camera::ComputeView()
{
	// Set the View Matrix
	mat4x4_mul(m_v, m_t, m_r);
}

void Camera::Translate(float stepX, float stepY, float stepZ)
{
	// Change the Position
	m_pos[0] += stepX;
	m_pos[1] += stepY;
	m_pos[2] += stepZ;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	// Update the Bounding Box
	//updateBoundingBox(m_box, m_pos, m_radius);
}

void Camera::Move(vec2& moveDir)
{
	// Translate the Actor
	float speed = 0.1f;
	this->Translate(moveDir[0] * speed, 0.0f, moveDir[1] * speed);
}

void Camera::Orbit(const vec2& mDelta)
{
	// Change the Yaw and Pitch
	m_yaw += mDelta[0] * 0.1f;
	m_pitch += mDelta[1] * 0.1f;
	// Clamp the Pitch
	m_pitch = clampf(m_pitch, PITCH_MIN, PITCH_MAX);
	
	// Set the new Direction
	m_dirF[0] = sin(m_yaw) * cos(m_pitch);
	m_dirF[1] = sin(m_pitch);
	m_dirF[2] = -(cos(m_yaw) * cos(m_pitch));
	
	// Normalize
	vec3_norm(m_dirF, m_dirF);
	
	// Get the Center
	memcpy(m_center, m_pParent->GetPos(), sizeof(vec3));
	
	// Set the Position from the Center
	m_pos[0] = m_center[0] + m_dirF[0] * m_dist;
	m_pos[1] = m_center[1] + m_dirF[1] * m_dist;
	m_pos[2] = m_center[2] + m_dirF[2] * m_dist;
}

void Camera::Update()
{
	this->ComputeView();
}
