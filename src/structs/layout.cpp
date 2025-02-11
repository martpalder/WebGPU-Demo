#include "./layout.hpp"

WGPUVertexAttribute createAttribVert(int idx)
{
	WGPUVertexAttribute attrib = {};
	
	// {{Describe the position attribute}}
	// == For each attribute, describe its layout, i.e., how to interpret the raw data ==
	// Means vec2f in the shader
	attrib.format = WGPUVertexFormat_Float32x2;
	// Index of the first element
	attrib.offset = idx;
	// Corresponds to @location(...)
	attrib.shaderLocation = idx;
	
	return attrib;
}

WGPUVertexBufferLayout createBufferLayoutVert(const WGPUVertexAttribute* pPosAttrib)
{
	WGPUVertexBufferLayout bufferLayout = {};
	
	/// Describe the Buffer Layout
	// {{Describe buffer stride and step mode}}
	bufferLayout.stepMode = WGPUVertexStepMode_Vertex;
	bufferLayout.arrayStride = 2 * sizeof(float);
	// {{Set Attributes}}
	if (pPosAttrib != nullptr)
	{
		bufferLayout.attributeCount = 1;
		bufferLayout.attributes = pPosAttrib;
	}
	
	return bufferLayout;
}
