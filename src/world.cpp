#include "./world.hpp"

#include <cstdio>

World::World()
{
	puts("Created the World");
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
	m_actors.push_back(new Actor());
	puts("Added an Actor");
	
	return m_actors.back();
}

void World::Update(const WGPUQueue& queue)
{
	for (Actor* pActor : m_actors)
	{
		pActor->Update(queue);
	}
}

void World::Draw(const WGPURenderPassEncoder& renderPass)
{
	for (Actor* pActor : m_actors)
	{
		pActor->Draw(renderPass);
	}
}
