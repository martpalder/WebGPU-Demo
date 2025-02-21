#pragma once
#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "./mesh.hpp"
#include "./bounding_box.hpp"
#include "./stdafx.h"

class Actor
{
private:
	// Members
	vec3 m_pos, m_radius;
	float m_speed, m_yaw;
	BoundingBox m_box;
	const char* m_tag;
	
	// Layouts
	WGPUBindGroupLayoutEntry m_bindingLayout;
	WGPUBindGroupLayout m_bindGroupLayout;
	
	// Components
	// Matrices
	mat4x4 m_t, m_r;
	mat4x4 m_m, m_mvp;
	// Buffers
	WGPUBuffer m_mvpBuffer;
	// Bind Group
	WGPUBindGroup m_bindGroup;
	
	// Mesh
	Mesh* m_pMesh;
	
	// Methods
	void Release();
	void CreateBindGroup(const WGPUDevice& device,
	const WGPUBindGroupLayout& bindGroupLayout);
	void SetBindGroup(const WGPURenderPassEncoder& renderPass);
	void ComputeMVP(const mat4x4& vp);

public:
	// Constructor/Destructor
	Actor(float x, float y, float z);
	~Actor();
	
	// Init
	void Init(const GPUEnv& gpuEnv,
	const WGPUBindGroupLayout& bindGroupLayout);
	
	// Getters
	const vec3& GetPos() const;
	const char* GetTag() const;
	const BoundingBox& GetBounds() const;
	WGPUBool IsColliding(const BoundingBox& other) const;
	WGPUBool CompareTag(const char* tag) const;
	
	// Setters
	void SetPos(float x, float y, float z);
	void SetPos(const vec3& pos);
	void SetYaw(float yaw);
	void SetTag(const char* tag);
	void SetMesh(Mesh* pMesh);
	void SetBoundingBox(const vec3& radius);
	
	// Main Methods
	void Update(const mat4x4& vp);
	void Draw(const WGPUQueue& queue,
	const WGPURenderPassEncoder& renderPass);
	
	// Transformations
	void TranslateH(float dirX, float dirZ);
	void MoveAndCollide(vec2& moveDir);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	
	// Load
	void LoadMesh(const GPUEnv& gpuEnv, const char* fileName);
	
	// Print
	void PrintPos();
};

#endif	// ACTOR_HPP_INCLUDED
