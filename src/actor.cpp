#include "./actor.hpp"
#include "./buffer.hpp"

Actor::Actor()
{
	// Set the Defaults
	this->SetDefaults();
	
	// Set the Projection Matrix
	mat4x4_perspective(m_proj, 90.0f, 4 / 3.0f, 0.1f, 100.0f);
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

Actor::~Actor()
{
	// Release the Actor
	this->Release();
}

WGPUBuffer& Actor::GetTransformBuffer()
{
	return m_mpBuffer;
}

void Actor::SetDefaults()
{
	// Set Default Values
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = -1.0f;
	
	// Initialize Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_model);
	mat4x4_identity(m_mp);
	
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

void Actor::Release()
{
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

void Actor::Update(const WGPUQueue& queue, const WGPURenderPassEncoder& renderPass)
{
	// Combine the Matrices
	mat4x4_rotate(m_r, m_r, 0.0f, 0.0f, 1.0f, 0.01f);
	mat4x4_mul(m_model, m_t, m_r);
	mat4x4_mul(m_mp, m_proj, m_model);	// Check ordering
	
	// Set the Mesh Buffers
	m_pMesh->SetBuffers(renderPass);
	// Update the Uniform Buffer
	wgpuQueueWriteBuffer(queue, m_mpBuffer, 0, m_mp, sizeof(m_mp));
}

void Actor::Translate(float x, float y, float z)
{
	// Change the Position
	m_pos[0] += x;
	m_pos[1] += y;
	m_pos[2] += z;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

void Actor::RotateZ(float z)
{
	mat4x4_rotate(m_r, m_r, 0.0f, 0.0f, 1.0f, z);
}

void Actor::Draw(const WGPURenderPassEncoder& renderPass)
{
	// If Indexed
	if (m_pMesh->IsIndexed())
	{
		// Draw 1 Instance of a X-Indices Shape
		uint32_t indexCount = m_pMesh->GetIndexCount();
		wgpuRenderPassEncoderDraw(renderPass, indexCount, 1, 0, 0);
	}
	else
	{
		// Draw 1 Instance of a X-Vertices Shape
		uint32_t vertexCount = m_pMesh->GetVertexCount();
		wgpuRenderPassEncoderDraw(renderPass, vertexCount, 1, 0, 0);
	}
}

void Actor::LoadMesh(const GPUEnv& gpuEnv)
{
	// Load a Mesh
	m_pMesh = loadMesh(gpuEnv);
}
