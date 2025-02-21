#include "./obj_loader.hpp"
#include "./freader.hpp"
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
	size_t vertexSz = 0;
	while (tok != nullptr)
	{
		// Set the Start Location
		char* start = tok;
		
		// Get an Element Span
		size_t span = strcspn(start, " ");
		// Copy the Element
		copyLineN(el, start, span);
		// Set the Start Location
		start += span + 1;
		
		// Check the Element Type
		// Vertex
		if (tok[0] == 'v')
		{
			vertexSz = 0;
			// Until the End of String
			while (*(start - 1) != '\0')
			{
				// Add a Vertex Float
				pMesh->AddVertexFloat(extractFloat(el, &start));
				vertexSz += 4;
			}
		}
		// Fragment
		if (tok[0] == 'f')
		{
			// Until the End of String
			while (*(start - 1) != '\0')
			{
				// Add an Index
				pMesh->AddIndex(extractUInt16(el, &start));
			}
		}
		
		// Take the Next Line
		tok = strtok(nullptr, "\n");
	}

	// Create the Buffers
	pMesh->CreateBuffers(gpuEnv, vertexSz, path);
	
	// Release the Text
	free(text);
	text = nullptr;
		
	return pMesh;
}
