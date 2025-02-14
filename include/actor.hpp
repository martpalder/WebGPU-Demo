#pragma once
#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "./mesh.hpp"

class Actor
{
private:
	// Members
	vec3 m_pos;
	Mesh m_mesh;
	
	// Matrices
	mat4x4 m_model, m_proj, m_mp;
	
	// Buffers
	WGPUBuffer m_mpBuffer;
	
	// Bindings
	Bind m_bind;
	
	// Methods
	void Release();

public:
	Actor();
	~Actor();
	
	// Getters
	WGPUBindGroupLayout* GetBindGroupLayout();
	
	// Setters
	void SetDefaults();
	void SetPos(float x, float y, float z);
	void SetBindGroup(const WGPURenderPassEncoder& renderPass);
	
	// Methods
	void CreateBindings(const GPUEnv& gpuEnv);
	void Update(const WGPUQueue& queue, const WGPURenderPassEncoder& renderPass);
	void Draw(const WGPURenderPassEncoder& renderPass);
	void Translate(float x, float y, float z);
	void LoadMesh(const GPUEnv& gpuEnv);
};

#endif	// ACTOR_HPP_INCLUDED
