#pragma once
#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "./mesh.hpp"

class Actor
{
private:
	// Members
	vec3 m_pos;
	float m_speed;
	
	// Components
	// Matrices
	mat4x4 m_t, m_r;
	mat4x4 m_model, m_proj, m_mp;
	// Buffers
	WGPUBuffer m_mpBuffer;
	// Bind Group
	WGPUBindGroup m_bindGroup;
	
	// Mesh
	Mesh* m_pMesh;
	
	// Methods
	void Release();

public:
	Actor();
	~Actor();
	
	// Getters
	WGPUBuffer& GetTBuffer();
	
	// Setters
	void SetDefaults();
	void SetPos(float x, float y, float z);
	
	// Main Methods
	void InitMatrices();
	void CreateTransform(const GPUEnv& gpuEnv);
	void CreateBindGroup(const WGPUDevice& device, const WGPUBindGroupLayout& bindGroupLayout);
	void Update(const WGPUQueue& queue);
	void Draw(const WGPURenderPassEncoder& renderPass);
	
	// Transformations
	void Translate(float x, float y, float z);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	
	// Load
	void LoadMesh(const GPUEnv& gpuEnv, const char* fileName);
};

#endif	// ACTOR_HPP_INCLUDED
