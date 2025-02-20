#include "./world.hpp"
#include "./stdafx.h"

World::World()
{
	// Setup Projection Matrix
	mat4x4_identity(m_p);
	mat4x4_identity(m_vp);
	mat4x4_perspective(m_p, 90.0f, 4 / 3.0f, 0.1f, 100.0f);
	
	// Update the Camera
	m_cam.Update();
	
	// Combine the View-Projection Matrix
	mat4x4_mul(m_vp, m_p, m_cam.GetView());
	puts("Created the World");
}

Camera* World::GetCam()
{
	return &m_cam;
}

void World::CreateBindGroups(const GPUEnv& gpuEnv,
const WGPUBindGroupLayout& bindGroupLayout)
{
	for (Actor* pActor : m_actors)
	{
		pActor->CreateBindGroup(gpuEnv, bindGroupLayout);
	}
	
	puts("Created the Bind Groups");
}

Actor* World::AddActor(const GPUEnv& gpuEnv, float x, float y, float z,
const char* tag)
{
	Actor* pActor = new Actor(x, y, z);
	pActor->Init(gpuEnv, m_vp);
	pActor->SetTag(tag);
	m_actors.push_back(pActor);
	printf("Added an Actor: '%s'\n", tag);
	
	return pActor;
}

void World::Update(const WGPUQueue& queue)
{
	// Update the Camera
	m_cam.Update();
	
	// Combine the View-Projection Matrix
	mat4x4_mul(m_vp, m_p, m_cam.GetView());
	
	// Update all Actors
	for (Actor* pActor : m_actors)
	{
		pActor->Update(queue, m_vp);
		
		if (pActor->CompareTag("Billboard"))
		{
			pActor->SetYaw(m_cam.GetYaw());
		}
	}
}

void World::Draw(const WGPURenderPassEncoder& renderPass)
{
	for (Actor* pActor : m_actors)
	{
		pActor->Draw(renderPass);
	}
}
