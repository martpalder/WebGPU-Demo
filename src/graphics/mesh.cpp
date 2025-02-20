#include "./mesh.hpp"
#include "./vertex.hpp"
#include "./buffer.hpp"
#include "./string_utils.hpp"
#include "./myassert.hpp"
#include "./stdafx.h"

const static float vertexData[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.5f,
};

Mesh::Mesh()
{
	m_vertexSz = 0;
	// Set Pointers
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
}

Mesh::~Mesh()
{
	// Release the Mesh
	this->Release();
}

void Mesh::Release()
{
	if (m_indexBuffer != nullptr)
	{
		// Release the Index Buffer
		wgpuBufferRelease(m_indexBuffer);
		m_indexBuffer = nullptr;
		puts("Released the Index Buffer");
	}
	if (m_vertexBuffer != nullptr)
	{
		// Release the Vertex Buffer
		wgpuBufferRelease(m_vertexBuffer);
		m_vertexBuffer = nullptr;
		puts("Released the Vertex Buffer");
	}
}

uint32_t Mesh::GetVertexCount()
{
	return m_vertices.size();
}

uint32_t Mesh::GetIndexCount()
{
	return m_indices.size();
}

WGPUBool Mesh::IsIndexed()
{
	return (m_indexBuffer != nullptr);
}

void Mesh::SetBuffers(const WGPURenderPassEncoder& renderPass)
{
	// Set the Vertex Buffer
	wgpuRenderPassEncoderSetVertexBuffer(
		renderPass,
		0,
		m_vertexBuffer,
		0,
		wgpuBufferGetSize(m_vertexBuffer)
	);
	
	// If is Indexed
	if (this->IsIndexed())
	{
		// Set the Index Buffer
		wgpuRenderPassEncoderSetIndexBuffer(
			renderPass,
			m_indexBuffer,
			WGPUIndexFormat_Uint16,
			0,
			wgpuBufferGetSize(m_indexBuffer)
		);
	}
	puts("Buffers");
}

void Mesh::CreateBuffers(const GPUEnv& gpuEnv, size_t vertexSz)
{
	// Set the Vertex Size
	m_vertexSz = vertexSz;
	
	// Caluclate the Data Sizes
	size_t vertexDataSz = m_vertices.size() * sizeof(vertexSz);
	size_t indexDataSz = m_indices.size() * sizeof(uint16_t);
	
	// Create the Vertex Buffer
	m_vertexBuffer = createBufferVert(gpuEnv.dev, vertexDataSz);
	// Upload the Geometry Data
	pushError(gpuEnv.dev);
	uploadToBuffer(gpuEnv.queue, m_vertexBuffer, vertexDataSz, &m_vertices[0]);
	popError(gpuEnv.dev);
	
	// If has Indices
	if (m_indices.size() > 0)
	{
		// Create the Index Buffer
		m_indexBuffer = createBufferIdx(gpuEnv.dev, indexDataSz);
		// Upload the Geometry Data
		pushError(gpuEnv.dev);
		uploadToBuffer(gpuEnv.queue, m_indexBuffer, indexDataSz, &m_indices[0]);
		popError(gpuEnv.dev);
	}
}

void Mesh::AddVertexFloat(const float vFloat)
{
	m_vertices.push_back(vFloat);
}

void Mesh::AddIndex(const uint16_t idx)
{
	m_indices.push_back(idx);
}

void Mesh::DrawTriangle(const WGPURenderPassEncoder& renderPass)
{
	// Set the Buffers
	this->SetBuffers(renderPass);
	
	// Draw 1 Instance of a 3-Vertices Shape
	wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);
}

void Mesh::Draw(const WGPURenderPassEncoder& renderPass)
{
	// Set the Buffers
	this->SetBuffers(renderPass);
	
	// If Indexed
	if (this->IsIndexed())
	{
		// Draw 1 Instance of a X-Indices Shape
		wgpuRenderPassEncoderDrawIndexed(renderPass, m_indices.size(), 1, 0, 0, 0);
	}
	else
	{
		// Draw 1 Instance of a X-Vertices Shape
		wgpuRenderPassEncoderDraw(renderPass, m_vertices.size(), 1, 0, 0);
	}
}

void Mesh::PrintInfo()
{
	putc('\n', stdout);
	puts("[MESH INFO]");
	printf("Vertex Count: %llu\n", m_vertices.size());
	printf("Vertex Buffer Size: %lld\n", wgpuBufferGetSize(m_vertexBuffer));
	if (m_indexBuffer != nullptr)
	{
		printf("Index Count: %llu\n", m_indices.size());
		printf("Index Buffer Size: %lld\n", wgpuBufferGetSize(m_indexBuffer));
	}
	putc('\n', stdout);
}

void Mesh::PrintData()
{
	putc('\n', stdout);
	puts("[MESH DATA]");
	for (const float vFloat : m_vertices)
	{
		printf("%f\n", vFloat);
	}
	putc('\n', stdout);
}
