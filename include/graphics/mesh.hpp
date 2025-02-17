#pragma once
#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./vertex.hpp"
#include "./bind.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

#define CUSTOM_INDEX uint16_t
#define INDEX_SZ sizeof(CUSTOM_INDEX)

class Mesh
{
private:
	// Members
	// Counts
	uint32_t m_vertexCount;
	uint32_t m_indexCount;
	// Buffer Sizes
	uint64_t m_vertexBufferSz;
	uint64_t m_indexBufferSz;
	// Buffers
	WGPUBuffer m_vertexBuffer;
	WGPUBuffer m_indexBuffer;
	// Datas
	const void* m_pVertexData;
	const void* m_pIndexData;
	
	// Methods
	void Release();
	WGPUBool IsIndexed();

public:
	// Constructor/Destructor
	Mesh();
	~Mesh();
	
	// Getters
	uint32_t GetVertexCount();
	uint32_t GetIndexCount();
	
	// Setters
	void SetDefaults();
	void SetBuffers(const WGPURenderPassEncoder& renderPass);
	void AssignVertices(const GPUEnv& gpuEnv, uint32_t vertexCount,
	const void* pVertexData);
	void AssignIndices(const GPUEnv& gpuEnv, uint32_t indexCount,
	const void* pIndexData);
	
	// Methods
	void Draw(const WGPURenderPassEncoder& renderPass);
};

#endif	// MESH_HPP_INCLUDED
