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
    // x0, y0, z0, color
    { -0.5f, -0.5f, 0.0f, RED },
    // x1, y1, z1, color
    { +0.5f, -0.5f, 0.0f, GREEN },
    // x2, y2, z2, color
    { +0.5f, +0.5f, 0.0f, BLUE },
	// x3, y3, z3, color
    { -0.5f, -0.5f, 0.0f, RED },
	// x4, y4, z4, color
    { +0.5f, +0.5f, 0.0f, BLUE },
	// x5, y5, z5, color
    { -0.5f, +0.5f, 0.0f, GREEN },
};

Mesh::Mesh()
{
	// Set Default Values
	this->SetDefaults();
}

Mesh::~Mesh()
{
	// Release
	this->Release();
}

uint32_t Mesh::GetVertexCount()
{
	return m_vertexCount;
}

void Mesh::SetDefaults()
{
	// Set Default Values
	m_vertexCount = 0;
	m_bufferSz = 0;
	m_pVertexData = nullptr;
	m_vertexBuffer = nullptr;
}

void Mesh::SetVertexBuffer(const WGPURenderPassEncoder& renderPass)
{
	// Set the Vertex Buffer
	wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, m_vertexBuffer, 0, m_bufferSz);
}

void Mesh::AssignVertexData(const GPUEnv& gpuEnv, uint32_t vertexCount,
const CUSTOM_VERTEX* pVertexData)
{
	// Set the Vertex Count and Vertex Data
	m_vertexCount = vertexCount;
	m_pVertexData = pVertexData;
	
	// Create a Vertex Buffer
	m_vertexBuffer = createBufferVert(gpuEnv, vertexCount * sizeof(CUSTOM_VERTEX), m_pVertexData);
	m_bufferSz = wgpuBufferGetSize(m_vertexBuffer);
	puts("Assigned Vertex Data");
	printf("Number of vertices: %u\n", m_vertexCount);
}

void Mesh::Release()
{
	if (m_vertexBuffer != nullptr)
	{
		// Release the Vertex Buffer
		/*wgpuBufferRelease(m_vertexBuffer);
		m_pVertexData = nullptr;
		m_vertexBuffer = nullptr;
		puts("Released the Vertex Buffer");*/
	}
}

Mesh loadMesh(const GPUEnv& gpuEnv)
{
	Mesh mesh;
	
	// Assign the Vertex Data
	uint32_t vertexCount = static_cast<uint32_t>(sizeof(vertexData) / VERTEX_SZ);
	mesh.AssignVertexData(gpuEnv, vertexCount, &vertexData[0]);
	
	return mesh;
}
