#pragma once
#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "./actor.hpp"
#include "./camera.hpp"

#include <vector>

class World
{
private:
	// Members
	Camera m_cam;
	std::vector<Actor*> m_actors;

public:
	// Constructor
	World();
	
	// Getters
	Camera* GetCam();
	
	// Create Functions
	void CreateBindGroups(const GPUEnv& gpuEnv,
	const WGPUBindGroupLayout& bindGroupLayout);
	
	// Add Functions
	Actor* AddActor(float x, float y, float z, const char* tag);
	
	// Main Functions
	void Update(const WGPUQueue& queue, const mat4x4& p);
	void Draw(const WGPURenderPassEncoder& renderPass);
};

#endif	// WORLD_HPP_INCLUDED
