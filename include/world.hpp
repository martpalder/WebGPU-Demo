#pragma once
#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "./actor.hpp"

#include <vector>

class World
{
private:
	// Members
	std::vector<Actor*> m_actors;

public:
	// Constructor
	World();
	
	// Create Functions
	void CreateBindGroups(const GPUEnv& gpuEnv,
	const WGPUBindGroupLayout& bindGroupLayout);
	
	// Add Functions
	Actor* AddActor(float x, float y, float z, const char* tag);
	
	// Main Functions
	void Update(const WGPUQueue& queue);
	void Draw(const WGPURenderPassEncoder& renderPass);
};

#endif	// WORLD_HPP_INCLUDED
