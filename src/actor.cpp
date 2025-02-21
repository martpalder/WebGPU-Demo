#include "./actor.hpp"
#include "./desc.hpp"
#include "./layout.hpp"
#include "./buffer.hpp"
#include "./mymath.h"
#include "./myassert.hpp"

Actor::Actor(float x, float y, float z)
{
	// Set Default Values
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
	m_speed = 0.2f;
	m_yaw = 0.0f;
	m_tag = "Actor";
	m_bindGroup = nullptr;
	m_pMesh = nullptr;
	
	// Initialize Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_m);
	mat4x4_identity(m_mvp);
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

Actor::~Actor()
{
	// Release the Actor
	this->Release();
}

void Actor::Init(const GPUEnv& gpuEnv,
const WGPUBindGroupLayout& bindGroupLayout)
{
	// Create the Uniform Buffer
	m_mvpBuffer = createBufferUniformMat(gpuEnv, m_mvp, m_tag);
	
	// Create a Bind Group
	this->CreateBindGroup(gpuEnv.dev, bindGroupLayout);
	printf("Initialized Actor: '%s'\n", m_tag);
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
	
	if (m_bindGroup != nullptr)
	{
		// Release the Bind Group
		wgpuBindGroupRelease(m_bindGroup);
		m_bindGroup = nullptr;
		puts("Released the Bind Group");
	}
	
	if (m_mvpBuffer != nullptr)
	{
		// Release the MVP Buffer
		wgpuBufferRelease(m_mvpBuffer);
		m_mvpBuffer = nullptr;
		puts("Released the MVP Buffer");
	}
}

const vec3& Actor::GetPos() const
{
	return m_pos;
}

const char* Actor::GetTag() const
{
	return m_tag;
}

const BoundingBox& Actor::GetBounds() const
{
	return m_box;
}

WGPUBool Actor::IsColliding(const BoundingBox& other) const
{
	return intersects(m_box, other);
}

WGPUBool Actor::CompareTag(const char* tag) const
{
	return (strcmp(m_tag, tag) == 0);
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

void Actor::SetPos(const vec3& pos)
{
	// Set the Position
	m_pos[0] = pos[0];
	m_pos[1] = pos[1];
	m_pos[2] = pos[2];
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
}

void Actor::SetBoundingBox(const vec3& radius)
{
	// Set the Box Radius
	copyVec3(m_radius, radius);
	// Update the Bounding Box
	updateBoundingBox(m_box, m_pos, radius);
}

void Actor::SetYaw(float yaw)
{
	// Set the Yaw
	m_yaw = -yaw;
	
	// Set the Rotation Matrix
	mat4x4_identity(m_r);
	mat4x4_rotate_Y(m_r, m_r, m_yaw);
}

void Actor::SetTag(const char* tag)
{
	m_tag = tag;
}

void Actor::SetMesh(Mesh* pMesh)
{
	if (pMesh != nullptr)
	{
		m_pMesh = pMesh;
		printf("Set the Mesh for '%s'\n", m_tag);
	}
}

void Actor::SetBindGroup(const WGPURenderPassEncoder& renderPass)
{
	if (m_bindGroup != nullptr)
	{
		// Set the Bind Group
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bindGroup, 0, nullptr);
	}
}

void Actor::CreateBindGroup(const WGPUDevice& device,
const WGPUBindGroupLayout& bindGroupLayout)
{
	// Create the Bindings
	WGPUBindGroupEntry bindings[] = {
		createBinding(0, m_mvpBuffer)
	};
	
	// Create a Bind Group Descriptor
	WGPUBindGroupDescriptor bindGroupDesc = {};
	bindGroupDesc = createBindGroupDesc(
		bindGroupLayout,
		1,
		&bindings[0],
		m_tag
	);
	
	// Create the Bind Group
	m_bindGroup = createBindGroup(device, bindGroupDesc);
	printf("Created a Bind Group for '%s'\n", m_tag);
}

void Actor::ComputeMVP(const mat4x4& vp)
{
	// Combine the Transformation Matrices
	mat4x4_mul(m_m, m_t, m_r);		// Check correct ordering
	// Combine the MVP Matrix
	mat4x4_mul(m_mvp, vp, m_m);		// Check correct ordering
}

void Actor::Update(const mat4x4& vp)
{
	// Compute the MVP Matrix
	this->ComputeMVP(vp);
}

void Actor::Draw(const WGPUQueue& queue,
const WGPURenderPassEncoder& renderPass)
{
	// {{Set the binding groups here!}}
	this->SetBindGroup(renderPass);
	
	// Upload the MVP to Shader
	wgpuQueueWriteBuffer(queue, m_mvpBuffer, 0, m_mvp, sizeof(m_mvp));
	
	// If has Mesh
	if (m_pMesh != nullptr)
	{
		// Draw the Mesh
		m_pMesh->Draw(renderPass);
	}
	else
	{
		perror("[ERROR]: No Mesh");
	}
}

void Actor::TranslateH(float dirX, float dirZ)
{
	// Change the Position
	m_pos[0] += dirX * m_speed;
	m_pos[2] += dirZ * m_speed;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	// Update the Bounding Box
	//updateBoundingBox(m_box, m_pos, m_radius);
}

void Actor::MoveAndCollide(vec2& moveDir)
{
	// Translate the Actor
	this->TranslateH(moveDir[0], moveDir[1]);
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

void Actor::PrintPos()
{
	printf("Pos X: %f Y: %f Z: %f\n", m_pos[0], m_pos[1], m_pos[2]);
}
