#pragma once
#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "./mesh.hpp"
#include "./stdafx.h"

class Actor
{
private:
	// Members
	vec3 m_pos;
	float m_speed;
	
	// Components
	// Matrices
	mat4x4 m_t, m_r, m_s;
	mat4x4 m_model, m_mvp;
	// Buffers
	WGPUBuffer m_mvpBuffer;
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
	
	// Getters
	vec3& GetPos();
	
	// Setters
	void SetPos(float x, float y, float z);
	void SetPos(const vec3& pos);
	void SetMesh(Mesh* pMesh);
	
	// Main Methods
	void Init();
	void CreateTransform(const GPUEnv& gpuEnv);
	void CreateBindGroup(const GPUEnv& gpuEnv, const WGPUBindGroupLayout& bindGroupLayout);
	void Update(const WGPUQueue& queue, const mat4x4& vp);
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
