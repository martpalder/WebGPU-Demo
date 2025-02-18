#include "./obj_loader.hpp"
#include "./color.hpp"
#include "./stdafx.h"

Mesh* loadOBJ(const GPUEnv& gpuEnv, const char* path)
{
	Mesh* pMesh = new Mesh();
	
	// Read the File Contents
	char* text = readFileText(path);
	char el[8];
	
	// Mesh Data
	std::vector<float> vertices;
	std::vector<uint16_t> indices;
	
	// Split the Text by Lines
	char* tok = strtok(text, "\n");
	while (tok != nullptr)
	{
		// Set the Start Location
		char* start = tok;
		
		// Get an Element Span
		size_t span = strcspn(start, " \n");
		// Copy the Element
		copyLineN(el, start, span);
		// Move the Start Location
		start += span + 1;
		
		// Check the Element Type
		// Vertex
		if (tok[0] == 'v')
		{
			// Add the Vertex with 6 Floats
			for (uint8_t i = 0; i < 6; ++i)
			{
				vertices.push_back(extractFloat(el, &start));
			}
		}
		// Fragment
		if (tok[0] == 'f')
		{
			// Add 3 Indices(A Triangle)
			for (uint8_t i = 0; i < 3; ++i)
			{
				indices.push_back(extractUInt16(el, &start));
			}
		}
		
		// Take the Next Line
		tok = strtok(nullptr, "\n");
	}

	// Assign the Vertices
	uint32_t vertexCount = (uint32_t)vertices.size();
	pMesh->AssignVertices(gpuEnv, vertexCount, &vertices[0]);
	
	// Assign the Indices
	uint32_t indexCount = (uint32_t)indices.size();
	pMesh->AssignIndices(gpuEnv, indexCount, &indices[0]);
	
	// Free the Text
	free(text);
		
	return pMesh;
}
