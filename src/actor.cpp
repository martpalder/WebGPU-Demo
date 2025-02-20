#include "./actor.hpp"
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
	m_bindGroup = nullptr;
	m_pMesh = nullptr;
	
	// Initialize Matrices
	mat4x4_identity(m_t);
	mat4x4_identity(m_r);
	mat4x4_identity(m_model);
	mat4x4_identity(m_mvp);
}

Actor::~Actor()
{
	// Release the Actor
	this->Release();
}

void Actor::Init(const GPUEnv& gpuEnv, const mat4x4& vp)
{
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	// Combine the Transformation Matrices
	mat4x4_mul(m_model, m_t, m_r);	// Check correct ordering
	// Combine the View-Projection and the Model
	mat4x4_mul(m_mvp, vp, m_model);	// Check correct ordering
	
	// Create and set the Uniform Buffer
	/*m_mvpBuffer = createBufferMatrix(gpuEnv, m_mvp);
	pushError(gpuEnv.dev);
	wgpuQueueWriteBuffer(gpuEnv.queue, m_mvpBuffer, 0, m_mvp, sizeof(mat4x4));
	popError(gpuEnv.dev);*/
}

vec3& Actor::GetPos()
{
	return m_pos;
}

const char* Actor::GetTag()
{
	return m_tag;
}

const BoundingBox& Actor::GetBounds()
{
	return m_box;
}

WGPUBool Actor::IsColliding(const BoundingBox& other)
{
	return intersects(m_box, other);
}

WGPUBool Actor::CompareTag(const char* tag)
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

void Actor::CreateBindGroup(const GPUEnv& gpuEnv,
const WGPUBindGroupLayout& bindGroupLayout)
{
	// Create the Bindings
	WGPUBindGroupEntry bindings[] = {
		createBinding(0, m_mvpBuffer),
	};
	
	// Create the Bind Group
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

	if (m_mvpBuffer != nullptr)
	{
		// Release the Transform Buffer
		wgpuBufferRelease(m_mvpBuffer);
		m_mvpBuffer = nullptr;
		puts("Released the Transform Buffer");
	}
}

void Actor::Update(const WGPUQueue& queue, const mat4x4& vp)
{
	// Combine the Transformation Matrices
	mat4x4_mul(m_model, m_t, m_r);	// Check correct ordering
	// Combine the View-Projection Matrix
	mat4x4_mul(m_mvp, vp, m_t);	// Check correct ordering
	
	// If has MVP Buffer
	/*if (m_mvpBuffer != nullptr)
	{
		// Update the Uniform Buffer
		wgpuQueueWriteBuffer(queue, m_mvpBuffer, 0, m_t, sizeof(mat4x4));
	}*/
}

void Actor::Draw(const WGPURenderPassEncoder& renderPass)
{
	// {{Set the binding groups here!}}
	if (m_bindGroup != nullptr)
	{
		// Set the Bind Group
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bindGroup, 0, nullptr);
	}
	else
	{
		perror("[ERROR]: No Bind Group");
	}
	
	// If has Mesh
	if (m_pMesh != nullptr)
	{
		// Draw the Mesh
		m_pMesh->Draw(renderPass);;
	}
	else
	{
		perror("[ERROR]: No Mesh");
	}
	
	// Draw a Triangle
	//m_pMesh->DrawTriangle(renderPass);
}

void Actor::Translate(float stepX, float stepY, float stepZ)
{
	// Change the Position
	m_pos[0] += stepX;
	m_pos[1] += stepY;
	m_pos[2] += stepZ;
	
	// Set the Translation Matrix
	mat4x4_translate(m_t, m_pos[0], m_pos[1], m_pos[2]);
	// Update the Bounding Box
	updateBoundingBox(m_box, m_pos, m_radius);
}

void Actor::MoveAndCollide(vec2& moveDir)
{
	// Normalize the Move Direction
	vec2_norm(moveDir, moveDir);
	// Translate the Actor
	this->Translate(moveDir[0] * m_speed, 0.0f, moveDir[1] * m_speed);
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
