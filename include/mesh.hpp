#pragma once
#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "./vertex.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

// Calculate Vertex Size
const static size_t VERTEX_SZ = sizeof(CUSTOM_VERTEX);

class Mesh
{
private:
	// Members
	uint32_t m_vertexCount;
	uint64_t m_bufferSz;
	WGPUBuffer m_vertexBuffer;
	mat4x4 m_model;
	const float* m_pVertexData;
	
	// Methods
	void Release();

public:
	// Constructor/Destructor
	Mesh();
	~Mesh();
	
	// Methods
	void Load(const WGPUDevice& device, const WGPUQueue& queue);
	void Update(const WGPURenderPassEncoder& renderPass);
	void Draw(const WGPURenderPassEncoder& renderPass);
};

#endif	// MESH_HPP_INCLUDED
