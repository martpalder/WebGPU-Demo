#include "./mesh.hpp"
#include "./vertex.hpp"
#include "./buffer.hpp"

#include <cstdio>

Mesh::Mesh()
{
	// Set Default Values
	this->SetDefaults();
}

Mesh::~Mesh()
{
	// Release the Mesh
	this->Release();
}

uint32_t Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t Mesh::GetIndexCount()
{
	return m_indexCount;
}

WGPUBool Mesh::IsIndexed()
{
	return (m_indexBuffer != nullptr);
}

void Mesh::SetDefaults()
{
	// Numbers
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertexBufferSz = 0;
	m_indexBufferSz = 0;
	// Pointers
	m_pVertexData = nullptr;
	m_vertexBuffer = nullptr;
	m_pIndexData = nullptr;
	m_indexBuffer = nullptr;
}

void Mesh::SetBuffers(const WGPURenderPassEncoder& renderPass)
{
	// Set the Vertex Buffer
	wgpuRenderPassEncoderSetVertexBuffer(
		renderPass, 0,
		m_vertexBuffer,
		0, m_vertexBufferSz
	);
	
	// If Indexed
	if (this->IsIndexed())
	{
		// Set the Index Buffer
		wgpuRenderPassEncoderSetIndexBuffer(
			renderPass,
			m_indexBuffer,
			WGPUIndexFormat_Uint16,
			0, m_indexBufferSz
		);
	}
}

void Mesh::AssignVertices(const GPUEnv& gpuEnv, uint32_t vertexCount,
const void* pVertexData)
{
	// Set the Vertex Count and Vertex Data
	m_vertexCount = vertexCount;
	m_pVertexData = pVertexData;
	
	// Caluclate Data Size
	size_t dataSize = vertexCount * sizeof(CUSTOM_VERTEX);
	
	// Create a Vertex Buffer
	m_vertexBuffer = createBufferVert(gpuEnv, dataSize, m_pVertexData);
	m_vertexBufferSz = wgpuBufferGetSize(m_vertexBuffer);
	puts("Assigned Vertex Data");
	printf("Number of vertices: %u\n", vertexCount);
}

void Mesh::AssignIndices(const GPUEnv& gpuEnv, uint32_t indexCount,
const void* pIndexData)
{
	// Set the Index Count and Index Data
	m_indexCount = indexCount;
	m_pIndexData = pIndexData;
	
	// Caluclate Data Size
	size_t dataSize = indexCount * sizeof(CUSTOM_INDEX);
	
	// Create an Index Buffer
	m_indexBuffer = createBufferIdx(gpuEnv, dataSize, m_pIndexData);
	m_indexBufferSz = wgpuBufferGetSize(m_indexBuffer);
	puts("Assigned Index Data");
	printf("Number of indices: %u\n", indexCount);
}

void Mesh::Release()
{
	if (m_indexBuffer != nullptr)
	{
		// Release the Index Buffer
		wgpuBufferRelease(m_indexBuffer);
		m_indexBuffer = nullptr;
		m_pIndexData = nullptr;
		puts("Released the Index Buffer");
	}
	if (m_vertexBuffer != nullptr)
	{
		// Release the Vertex Buffer
		wgpuBufferRelease(m_vertexBuffer);
		m_vertexBuffer = nullptr;
		m_pVertexData = nullptr;
		puts("Released the Vertex Buffer");
	}
}

void Mesh::Draw(const WGPURenderPassEncoder& renderPass)
{
	// Set the Buffers
	this->SetBuffers(renderPass);
	
	// If Indexed
	if (this->IsIndexed())
	{
		// Draw 1 Instance of a X-Indices Shape
		wgpuRenderPassEncoderDrawIndexed(renderPass, m_indexCount, 1, 0, 0, 0);
	}
	else
	{
		// Draw 1 Instance of a X-Vertices Shape
		wgpuRenderPassEncoderDraw(renderPass, m_vertexCount, 1, 0, 0);
	}
}
