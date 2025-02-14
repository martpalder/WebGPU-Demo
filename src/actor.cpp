#include "./actor.hpp"
#include "./buffer.hpp"

Actor::Actor()
{
	// Set the Projection Matrix
	mat4x4_perspective(m_proj, 90.0f, 4 / 3.0f, 0.1f, 100.0f);
	// Set the Model Matrix
	mat4x4_translate(m_model, 1.0f, 0.0f, -1.0f);
	
	// Combine the Matrices
	mat4x4_mul(m_mp, m_proj, m_model);	// Check ordering
}

Actor::~Actor()
{
	// Release
	this->Release();
}

WGPUBindGroupLayout* Actor::GetBindGroupLayout()
{
	return &m_bind.bindGroupLayout;
}

void Actor::SetDefaults()
{
	// Set Default Values
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = -1.0f;
	
	// Initialize Matrices
	mat4x4_identity(m_model);
	mat4x4_identity(m_mp);
	
	// Bindings
	m_bind.binding = {};
	m_bind.bindingLayout = {};
	m_bind.bindGroupLayout = nullptr;
	m_bind.bindGroup = nullptr;
}

void Actor::SetPos(float x, float y, float z)
{
	// Set the Position
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
	
	// Set the Model Matrix
	mat4x4_translate(m_model, m_pos[0], m_pos[1], m_pos[2]);
	// Combine the Matrices
	mat4x4_mul(m_mp, m_proj, m_model);	// Check ordering
}

void Actor::Release()
{
	// Release the Bindings
	if (m_bind.bindGroup != nullptr)
	{
		wgpuBindGroupRelease(m_bind.bindGroup);
		m_bind.bindGroup = nullptr;
		puts("Released the Bind Group");
	}
	if (m_bind.bindGroupLayout != nullptr)
	{
		wgpuBindGroupLayoutRelease(m_bind.bindGroupLayout);
		m_bind.bindGroupLayout = nullptr;
		puts("Released the Bind Group Layout");
	}
	
	// Release the Buffers
	if (m_mpBuffer != nullptr)
	{
		// Release the Transform Buffer
		wgpuBufferRelease(m_mpBuffer);
		m_mpBuffer = nullptr;
		puts("Released the Transform Buffer");
	}
}

void Actor::CreateBindings(const GPUEnv& gpuEnv)
{
	// Create and bind the Transform Buffer
	m_mpBuffer = createBufferMatrix(gpuEnv, m_mp);
	m_bind = bindBuffer(gpuEnv.dev, 0, m_mpBuffer);
}

void Actor::Update(const WGPUQueue& queue, const WGPURenderPassEncoder& renderPass)
{
	//this->Translate(0.0f, 0.0f, -0.01f);
	
	// Set the Vertex Buffer
	m_mesh.SetVertexBuffer(renderPass);
	// Update the Uniform Buffer
	wgpuQueueWriteBuffer(queue, m_mpBuffer, 0, m_mp, sizeof(m_mp));
	
	// {{Set the binding group here!}}
	if (m_bind.bindGroup != nullptr)
	{
		// Set the Bind Group
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bind.bindGroup, 0, nullptr);
	}
}

void Actor::Draw(const WGPURenderPassEncoder& renderPass)
{
	// Draw 1 Instance of a X-Vertices Shape
	uint32_t vertexCount = m_mesh.GetVertexCount();
	wgpuRenderPassEncoderDraw(renderPass, vertexCount, 1, 0, 0);
}

void Actor::Translate(float x, float y, float z)
{
	// Change the Position
	m_pos[0] += x;
	m_pos[1] += y;
	m_pos[2] += z;
	
	// Set the Model Matrix
	mat4x4_translate(m_model, m_pos[0], m_pos[1], m_pos[2]);
	// Combine the Matrices
	mat4x4_mul(m_mp, m_proj, m_model);	// Check ordering
}

void Actor::LoadMesh(const GPUEnv& gpuEnv)
{
	m_mesh = loadMesh(gpuEnv);
}
