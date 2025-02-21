#pragma once
#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "./desc.hpp"
#include "./attach.hpp"
#include "./layout.hpp"
#include "./camera.hpp"
#include "./actor.hpp"
#include "./stdafx.h"

class World
{
private:
	// Members
	// Attachments and Descriptors
	Attachments m_attach;
	Descriptors m_desc;
	// Layouts
	Layouts m_layouts;
	// Pipeline
	WGPURenderPipeline m_pipeline;
	// Depth
	WGPUTexture m_depthTexture;
	WGPUTextureView m_depthView;
	// Target Texture View
	WGPUTextureView m_targetView;
	
	// Camera and Matrices
	Camera m_cam;
	mat4x4 m_p, m_vp;
	
	// Actors
	std::vector<Actor*> m_actors;
	
	// Methods
	void Release();
	void RunRenderPass(const WGPUQueue& queue,
	const WGPUCommandEncoder& encoder);
	void SubmitCommand(const WGPUQueue& queue,
	const WGPUCommandBuffer& command);
	
	// Create Functions
	void CreateLayouts(const WGPUDevice& device);
	void CreatePipeline(const WGPUDevice& device,
	const WGPUShaderModule& shaderMod, int w, int h);

public:
	// Constructor
	World();
	~World();
	
	// Init
	void Init(const WGPUDevice& device,
	const WGPUShaderModule& shaderMod,
	int w, int h);
	
	// Getters
	Camera* GetCam();
	
	// Add Functions
	Actor* AddActor(const GPUEnv& gpuEnv, float x, float y, float z,
	const char* tag);
	
	// Main Functions
	void Cls(const WGPUSurface& surf);
	void Flip(const WGPUSurface& surf);
	void Update();
	void Draw(const WGPUDevice& device, const WGPUQueue& queue);
};

#endif	// WORLD_HPP_INCLUDED
