#pragma once
#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "./mesh.hpp"

class Actor
{
private:
	// Members
	vec3 m_pos;
	
	// Components
	// Matrices
	mat4x4 m_t, m_r;
	mat4x4 m_model, m_proj, m_mp;
	// Buffers
	WGPUBuffer m_mpBuffer;
	// Mesh
	Mesh* m_pMesh;
	
	// Methods
	void Release();

public:
	Actor();
	~Actor();
	
	// Getters
	WGPUBuffer& GetTransformBuffer();
	
	// Setters
	void SetDefaults();
	void SetPos(float x, float y, float z);
	
	// Main Methods
	void CreateTransform(const GPUEnv& gpuEnv);
	void Update(const WGPUQueue& queue, const WGPURenderPassEncoder& renderPass);
	void Draw(const WGPURenderPassEncoder& renderPass);
	
	// Transformations
	void Translate(float x, float y, float z);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	
	// Load
	void LoadMesh(const GPUEnv& gpuEnv);
};

#endif	// ACTOR_HPP_INCLUDED
