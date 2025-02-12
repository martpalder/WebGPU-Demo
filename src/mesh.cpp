#include "./mesh.hpp"
#include "./buffer.hpp"

#include <cstdio>

// Vertex buffer data
// There are 2 floats per vertex, one for x and one for y.
// But in the end this is just a bunch of floats to the eyes of the GPU,
// the *layout* will tell how to interpret this.
const static float vertexData[] = {
    // x0, y0, z0
    -0.5f, -0.5f, 0.0f,
    // x1, y1, z1
    +0.5f, -0.5f, 0.0f,
    // x2, y2, z2
    +0.5f, +0.5f, 0.0f,
	// x3, y3, z3
    -0.5f, -0.5f, 0.0f,
	// x4, y4, z4
    +0.5f, +0.5f, 0.0f,
	// x5, y5, z5
    -0.5f, +0.5f, 0.0f,
};

Mesh::Mesh()
{
	m_vertexCount = 0;
	m_bufferSz = 0;
	
	m_pVertexData = nullptr;
	m_vertexBuffer = nullptr;
	
	// Set the Model Matrix
	mat4x4_translate(m_model, 0.0f, 0.0f, 0.0f);
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
	m_pVertexData = &vertexData[0];
	
	// Create a Vertex Buffer
	m_vertexBuffer = createBufferVert(device, queue, sizeof(vertexData), m_pVertexData);
	m_bufferSz = wgpuBufferGetSize(m_vertexBuffer);
}

void Mesh::Draw(const WGPURenderPassEncoder& renderPass)
{
	if (m_vertexBuffer != nullptr)
	{
		// Set the Vertex Buffer
		wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, m_vertexBuffer, 0, m_bufferSz);
		// Draw 1 Instance of a X-Vertices Shape
		wgpuRenderPassEncoderDraw(renderPass, m_vertexCount, 1, 0, 0);
	}
}
