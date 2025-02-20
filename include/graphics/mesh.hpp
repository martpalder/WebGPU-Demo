#pragma once
#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./vertex.hpp"
#include "./bind.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

#define CUSTOM_VERTEX Vertex2D_Pos
#define VERTEX_SZ sizeof(CUSTOM_VERTEX)

class Mesh
{
private:
	// Members
	size_t m_vertexSz;
	// Buffers
	WGPUBuffer m_vertexBuffer;
	WGPUBuffer m_indexBuffer;
	// Datas
	std::vector<float> m_vertices;
	std::vector<uint16_t> m_indices;
	
	// Setters
	void SetBuffers(const WGPURenderPassEncoder& renderPass);
	
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
	
	// Main Methods
	void CreateBuffers(const GPUEnv& gpuEnv, size_t vertexSz);
	void AddVertexFloat(const float vFloat);
	void AddIndex(const uint16_t idx);
	void DrawTriangle(const WGPURenderPassEncoder& renderPass);
	void Draw(const WGPURenderPassEncoder& renderPass);
	
	// Print
	void PrintInfo();
	void PrintData();
};

#endif	// MESH_HPP_INCLUDED
