#include "./obj_loader.hpp"
#include "./color.hpp"

#include <cstdio>
#include <vector>

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
			// Get the X
			ExtFloat extX = extractFloat(el, start);
			start += extX.span + 1;
			// Get the Y
			ExtFloat extY = extractFloat(el, start);
			start += extY.span + 1;
			// Get the Z
			ExtFloat extZ = extractFloat(el, start);
			start += extZ.span + 1;
			
			// Add the Position
			vertices.push_back(extX.val);
			vertices.push_back(extY.val);
			vertices.push_back(extZ.val);
			
			// Get the Red
			ExtFloat extR = extractFloat(el, start);
			start += extR.span + 1;
			// Get the Green
			ExtFloat extG = extractFloat(el, start);
			start += extG.span + 1;
			// Get the Blue
			ExtFloat extB = extractFloat(el, start);
			start += extB.span + 1;
			
			// Add the Color
			vertices.push_back(extR.val);
			vertices.push_back(extG.val);
			vertices.push_back(extB.val);
		}
		// Fragment
		if (tok[0] == 'f')
		{
			// Get the Index0
			ExtUInt16 extIdx0 = extractUInt16(el, start);
			start += extIdx0.span + 1;
			// Get the Index1
			ExtUInt16 extIdx1 = extractUInt16(el, start);
			start += extIdx1.span + 1;
			// Get the Index2
			ExtUInt16 extIdx2 = extractUInt16(el, start);
			start += extIdx2.span + 1;
			
			// Add the Indices
			indices.push_back(extIdx0.val);
			indices.push_back(extIdx1.val);
			indices.push_back(extIdx2.val);
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
