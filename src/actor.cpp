#include "./actor.hpp"
#include "./buffer.hpp"
#include "./obj_loader.hpp"

#include <cstring>

Actor::Actor()
{
	// Set the Defaults
	this->SetDefaults();
	this->InitMatrices();
	
	// Set the Projection Matrix
	float fov = 90.0f;
	mat4x4_perspective(m_proj, fov, 4 / 3.0f, 0.1f, 100.0f);
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
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

void Actor::SetDefaults()
{
	// Set Default Values
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = -1.5f;
	m_speed = 0.2f;
	m_bindGroup = nullptr;
	m_pMesh = nullptr;
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

void Actor::InitMatrices()
{
	// Initialize Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_model);
	mat4x4_identity(m_mp);
}

void Actor::CreateBindGroup(const WGPUDevice& device,
const WGPUBindGroupLayout& bindGroupLayout)
{
	// Bind the Transform Buffer
	WGPUBindGroupEntry bindings[] = {
		createBinding(0, m_mpBuffer),
	};
	m_bindGroup = createBindGroup(device, bindGroupLayout, 1, &bindings[0]);
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

void Actor::CreateTransform(const GPUEnv& gpuEnv)
{
	// Create the Transform Buffer
	m_mpBuffer = createBufferMatrix(gpuEnv, m_mp);
}

void Actor::Update(const WGPUQueue& queue)
{
	// Combine the Matrices
	mat4x4_mul(m_model, m_t, m_r);
	mat4x4_mul(m_mp, m_proj, m_model);	// Check ordering
	
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
	mat4x4_rotate(m_r, m_r, 1.0f, 0.0f, 0.0f, x);
}

void Actor::RotateY(float y)
{
	mat4x4_rotate(m_r, m_r, 0.0f, 1.0f, 0.0f, y);
}

void Actor::RotateZ(float z)
{
	mat4x4_rotate(m_r, m_r, 0.0f, 0.0f, 1.0f, z);
}

void Actor::LoadMesh(const GPUEnv& gpuEnv, const char* fileName)
{
	this->CreateTransform(gpuEnv);
	
	// Set the Filepath
	char path[32];
	sprintf(path, "./data/models/%s", fileName);
	
	// Get the File Extension
	const char* fileExt = strrchr(fileName, '.') + 1;
	
	// Check the File Extension
	if (strcmp(fileExt, "obj") == 0)
	{
		// Load an OBJ
		m_pMesh = loadOBJ(gpuEnv, path);
	}
}
