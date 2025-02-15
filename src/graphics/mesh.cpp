#include "./mesh.hpp"
#include "./vertex.hpp"
#include "./buffer.hpp"
#include "./color.hpp"

#include <cstdio>

// Vertex buffer data
// There are 3 floats per vertex,
// one for x and one for y and one for z.
// But in the end this is just a bunch of floats to the eyes of the GPU,
// the *layout* will tell how to interpret this.
const static CUSTOM_VERTEX vertexData[] =
{
	// Front
    { -0.5f, -0.5f, 0.0f, RED },
    { +0.5f, -0.5f, 0.0f, GREEN },
    { +0.5f, +0.5f, 0.0f, BLUE },
    { -0.5f, -0.5f, 0.0f, RED },
    { +0.5f, +0.5f, 0.0f, BLUE },
    { -0.5f, +0.5f, 0.0f, GREEN },
};

const static CUSTOM_INDEX indexData[] =
{
	0, 1, 2,
	0, 2, 3,
};

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
	// Set Default Values
	m_vertexCount = 0;
	m_indexCount = 0;
	
	m_vertexBufferSz = 0;
	m_indexBufferSz = 0;
	
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
	
	// Create a Vertex Buffer
	m_vertexBuffer = createBufferVert(gpuEnv, vertexCount * sizeof(CUSTOM_VERTEX), m_pVertexData);
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
	
	// Create an Index Buffer
	m_indexBuffer = createBufferIdx(gpuEnv, indexCount * sizeof(CUSTOM_INDEX), m_pIndexData);
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

Mesh* loadMesh(const GPUEnv& gpuEnv)
{
	Mesh* pMesh = new Mesh();
	
	// Assign the Vertices
	uint32_t vertexCount = (uint32_t)(sizeof(vertexData) / VERTEX_SZ);
	pMesh->AssignVertices(gpuEnv, 6, &vertexData[0]);
	
	// Assign the Indices
	//uint32_t indexCount = (uint32_t)(sizeof(indexData) / INDEX_SZ);
	//pMesh->AssignIndices(gpuEnv, indexCount, &indexData[0]);
	
	return pMesh;
}
