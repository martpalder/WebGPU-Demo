#include "./pipeline.hpp"
#include "./state.hpp"
#include "./desc.hpp"
#include "./attrib.hpp"
#include "./layout.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPURenderPipeline createRenderPipeline(const WGPUDevice& device, const WGPUShaderModule& shaderMod,
WGPUBindGroupLayout* pBindGroupLayout)
{
    WGPURenderPipeline pipeline;

	// Configure Pipeline States:
    // Configure vertex input, rasterization, and color blending states.
	WGPUBlendState blendState = createBlendState();
	WGPUColorTargetState colorTarget = createColorTargetState(blendState);
	
	// Create and set the Fragment State
	WGPUFragmentState fragmentState = createFragmentState(shaderMod, colorTarget);
	
	// Create a Render Pipeline Descriptor
    WGPURenderPipelineDescriptor pipelineDesc = createRenderPipelineDesc(shaderMod, fragmentState);
	
	// VERTEX FETCH
	// Create Attributes
	WGPUVertexAttribute attribs[] = {
		createAttribVertFloat(3, 0, 0, "position"),
		createAttribVertFloat(3, 1, 3 * sizeof(float), "color"),
	};
	size_t vertexSz = 6 * sizeof(float);
	
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = createLayoutBufferVert(vertexSz, 2, &attribs[0]);
	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &bufferLayout;
	
	// If Bind Group Layout not NULL
	if (pBindGroupLayout != nullptr)
	{
		// PIPELINE LAYOUT
		// Assign the PipelineLayout to the RenderPipelineDescriptor's layout field
		pipelineDesc.layout = createLayoutPipeline(device, pBindGroupLayout);
		pipelineDesc.label = "UniformMatPipeline";
	}

	// Create the Render Pipeline:
    // Use the device to create the render pipeline.
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Chek for Errors
	if (pipeline == nullptr)
	{
		perror("[ERROR]: Failed to create the Render Pipeline");
	}
	{
		puts("Created the Render Pipeline");
	}
	
	// Assert
	MY_ASSERT(pipeline != nullptr);

	// Use Pipeline in Command Buffers:
    // Integrate the pipeline in command buffers to execute rendering commands.
	
    return pipeline;
}
