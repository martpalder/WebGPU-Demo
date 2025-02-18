#include "./attrib.hpp"
#include "./stdafx.h"

WGPUVertexAttribute createAttribVertFloat(int numFloats, int idx, int offset, const char* label)
{
	WGPUVertexAttribute attrib = {};
	
	// {{Describe the position attribute}}
	// == For each attribute, describe its layout, i.e., how to interpret the raw data ==
	// Set the Corrrect Format
	switch(numFloats)
	{
		case 2: attrib.format = WGPUVertexFormat_Float32x2; break;
		case 3: attrib.format = WGPUVertexFormat_Float32x3; break;
	}
	// Offset from the first element
	attrib.offset = offset;
	// Corresponds to @location(...)
	attrib.shaderLocation = idx;
	printf("Created a Vertex Attribute%d: '%s'\n", idx, label);
	
	return attrib;
}
