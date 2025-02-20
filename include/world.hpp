#pragma once
#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "./camera.hpp"
#include "./actor.hpp"
#include "./stdafx.h"

class World
{
private:
	// Members
	Camera m_cam;
	mat4x4 m_p, m_vp;
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
	Actor* AddActor(const GPUEnv& gpuEnv, float x, float y, float z,
	const char* tag);
	
	// Main Functions
	void Update(const WGPUQueue& queue);
	void Draw(const WGPURenderPassEncoder& renderPass);
};

#endif	// WORLD_HPP_INCLUDED
