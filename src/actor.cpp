#include "./actor.hpp"
#include "./buffer.hpp"

#include <cstring>

Actor::Actor()
{
	// Initialize
	this->Init();
}

Actor::~Actor()
{
	// Release the Actor
	this->Release();
}

WGPUBuffer& Actor::GetTBuffer()
{
	return m_mpBuffer;
}

void Actor::Init()
{
	// Set Default Values
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = -1.5f;
	m_speed = 0.2f;
	m_bindGroup = nullptr;
	m_pMesh = nullptr;
	
	// Initialize Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_model);
	mat4x4_identity(m_proj);
	mat4x4_identity(m_mp);
}

void Actor::SetPos(float x, float y, float z)
{
	// Set the Position
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

void Actor::SetMesh(Mesh* pMesh)
{
	m_pMesh = pMesh;
}

void Actor::CreateBindGroup(const GPUEnv& gpuEnv,
const WGPUBindGroupLayout& bindGroupLayout)
{
	// Set the Projection Matrix
	float fov = 90.0f;
	mat4x4_perspective(m_proj, fov, 4 / 3.0f, 0.1f, 100.0f);
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	
	// Create the Transform Buffer
	m_mpBuffer = createBufferMatrix(gpuEnv, m_mp);
	
	// Bind the Transform Buffer
	WGPUBindGroupEntry bindings[] = {
		createBinding(0, m_mpBuffer),
	};
	m_bindGroup = createBindGroup(gpuEnv.dev, bindGroupLayout, 1, &bindings[0]);
}

void Actor::Release()
{
	if (m_bindGroup != nullptr)
	{
		// Release the Bind Group
		wgpuBindGroupRelease(m_bindGroup);
		m_bindGroup = nullptr;
		puts("Released the Bind Group");
	}
	
	if (m_pMesh != nullptr)
	{
		// Release the Mesh
		delete m_pMesh;
		m_pMesh = nullptr;
		puts("Released the Mesh");
	}

	if (m_mpBuffer != nullptr)
	{
		// Release the Transform Buffer
		wgpuBufferRelease(m_mpBuffer);
		m_mpBuffer = nullptr;
		puts("Released the Transform Buffer");
	}
}

void Actor::Update(const WGPUQueue& queue)
{
	// Combine the Transformation Matrices
	mat4x4_mul(m_model, m_t, m_r);
	// Combine Model, View and Projection
	mat4x4_mul(m_mp, m_proj, m_model);	// Check correct ordering
	
	// Update the Uniform Buffer
	wgpuQueueWriteBuffer(queue, m_mpBuffer, 0, m_mp, sizeof(m_mp));
}

void Actor::Draw(const WGPURenderPassEncoder& renderPass)
{
	// {{Set the binding groups here!}}
	if (m_bindGroup != nullptr)
	{
		// Set the Bind Group
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bindGroup, 0, nullptr);
	}
	
	// If has Mesh
	if (m_pMesh != nullptr)
	{
		// Draw the Mesh
		m_pMesh->Draw(renderPass);
	}
}

void Actor::Translate(float x, float y, float z)
{
	// Change the Position
	m_pos[0] += x * m_speed;
	m_pos[1] += y * m_speed;
	m_pos[2] += z * m_speed;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

void Actor::RotateX(float x)
{
	mat4x4_rotate_X(m_r, m_r, x);
}

void Actor::RotateY(float y)
{
	mat4x4_rotate_Y(m_r, m_r, y);
}

void Actor::RotateZ(float z)
{
	mat4x4_rotate_Z(m_r, m_r, z);
}
