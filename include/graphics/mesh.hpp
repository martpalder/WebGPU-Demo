#pragma once
#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./vertex.hpp"
#include "./bind.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

// Calculate Vertex Size
const static size_t VERTEX_SZ = sizeof(CUSTOM_VERTEX);

class Mesh
{
private:
	// Members
	// Counts
	uint32_t m_vertexCount;
	uint64_t m_bufferSz;
	// Buffers
	WGPUBuffer m_vertexBuffer;
	
	// Vertex Data
	const CUSTOM_VERTEX* m_pVertexData;
	
	// Methods
	void Release();

public:
	// Constructor/Destructor
	Mesh();
	~Mesh();
	
	// Getters
	uint32_t GetVertexCount();
	
	// Setters
	void SetDefaults();
	void SetVertexBuffer(const WGPURenderPassEncoder& renderPass);
	void AssignVertexData(const GPUEnv& gpuEnv, uint32_t vertexCount,
	const CUSTOM_VERTEX* pVertexData);
};

Mesh loadMesh(const GPUEnv& gpuEnv);

#endif	// MESH_HPP_INCLUDED
