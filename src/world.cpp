#include "./world.hpp"
#include "./stdafx.h"

World::World()
{
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

Actor* World::AddActor(float x, float y, float z, const char* tag)
{
	Actor* pActor = new Actor();
	
	pActor->SetPos(x, y, z);
	m_actors.push_back(pActor);
	printf("Added an Actor: '%s'\n", tag);
	
	return pActor;
}

void World::Update(const WGPUQueue& queue, const mat4x4& p)
{
	// Update the Camera
	m_cam.Update();
	
	// Combine the View-Projection Matrix
	mat4x4 vp;
	mat4x4_mul(vp, p, m_cam.GetView());
	
	// Update all Actors
	for (Actor* pActor : m_actors)
	{
		pActor->Update(queue, vp);
	}
}

void World::Draw(const WGPURenderPassEncoder& renderPass)
{
	for (Actor* pActor : m_actors)
	{
		pActor->Draw(renderPass);
	}
}
