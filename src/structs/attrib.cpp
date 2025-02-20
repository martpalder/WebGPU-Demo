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
		case 1: attrib.format = WGPUVertexFormat_Float32; break;
		case 2: attrib.format = WGPUVertexFormat_Float32x2; break;
		case 3: attrib.format = WGPUVertexFormat_Float32x3; break;
		case 4: attrib.format = WGPUVertexFormat_Float32x4; break;
	}
	// Offset from the first element
	attrib.offset = offset;
	// Corresponds to @location(...)
	attrib.shaderLocation = idx;
	printf("Created a Vertex attrib%d: '%s'\n", idx, label);
	
	return attrib;
}

WGPUVertexAttribute* attrib2DPos()
{
	WGPUVertexAttribute* pAttribs = new WGPUVertexAttribute[1];
	pAttribs[0] = createAttribVertFloat(2, 0, 0, "pos");
	
	return pAttribs;
}

WGPUVertexAttribute* attrib2DPosColor()
{
	WGPUVertexAttribute* pAttribs = new WGPUVertexAttribute[2];
	pAttribs[0] = createAttribVertFloat(2, 0, 0, "pos");
	pAttribs[1] = createAttribVertFloat(3, 1, 2 * sizeof(float), "color");
	
	return pAttribs;
}

WGPUVertexAttribute* attrib3DPos()
{
	WGPUVertexAttribute* pAttribs = new WGPUVertexAttribute[1];
	pAttribs[0] = createAttribVertFloat(3, 0, 0, "pos");
	
	return pAttribs;
}

WGPUVertexAttribute* attrib3DPosColor()
{
	WGPUVertexAttribute* pAttribs = new WGPUVertexAttribute[1];
	pAttribs[0] = createAttribVertFloat(3, 0, 0, "pos");
	pAttribs[1] = createAttribVertFloat(3, 1, 3 * sizeof(float), "color");
	
	return pAttribs;
}
