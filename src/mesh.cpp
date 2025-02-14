#include "./mesh.hpp"
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
	m_vertexCount = 0;
	m_bufferSz = 0;
	
	m_pVertexData = nullptr;
	m_vertexBuffer = nullptr;
	
	// Initilalize the Model Matrix
	mat4x4_identity(m_model);
}

Mesh::~Mesh()
{
	// Release the Vertex Buffer
	this->Release();
}

void Mesh::Release()
{
	// Release the Vertex Buffer
	wgpuBufferRelease(m_vertexBuffer);
	m_pVertexData = nullptr;
	m_vertexBuffer = nullptr;
	puts("Released the Vertex Buffer");
}

void Mesh::Load(const WGPUDevice& device, const WGPUQueue& queue)
{	
	// Set the Vertex Data
	m_vertexCount = static_cast<uint32_t>(sizeof(vertexData) / VERTEX_SZ);
	m_pVertexData = (float*)&vertexData[0];
	
	// Create a Vertex Buffer
	m_vertexBuffer = createBufferVert(device, queue, sizeof(vertexData), m_pVertexData);
	m_bufferSz = wgpuBufferGetSize(m_vertexBuffer);
	printf("Number of vertices: %u\n", m_vertexCount);
}

void Mesh::Update(const WGPURenderPassEncoder& renderPass)
{
	// Set the Vertex Buffer
	wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, m_vertexBuffer, 0, m_bufferSz);
}

void Mesh::Draw(const WGPURenderPassEncoder& renderPass)
{
	// Draw 1 Instance of a X-Vertices Shape
	wgpuRenderPassEncoderDraw(renderPass, m_vertexCount, 1, 0, 0);
}
