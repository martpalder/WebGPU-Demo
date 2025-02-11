#include "./app.hpp"

#include "./window.hpp"
#include "./graphics/desc.hpp"
#include "./graphics/config.hpp"
#include "./graphics/attach.hpp"
#include "./graphics/texture.hpp"
#include "./env/gpuenv.hpp"

#include <cstdio>

App::App()
{
	// INITIALIZATION
	// Create the window
	m_wnd = createWindow();
	if (m_wnd == nullptr){ this->~App(); }
	
	// Initialize WebGPU
	m_gpuEnv = initGPUEnv(m_wnd);
	
	// Configure the Surface
	WGPUSurfaceConfiguration config = createSurfConfig(m_gpuEnv.dev);
	wgpuSurfaceConfigure(m_gpuEnv.surf, &config);
	
	// Create the Attachments and Descriptors
	this->CreateAttachments();
	this->CreateDescriptors();
	this->LoadShader();
	
	// Create Render Pipeline
	this->CreateRenderPipeline();
	puts("Started the App");
}

App::~App()
{
	// Quit WebGPU
	quitGPUEnv(m_gpuEnv);
	// Destroy the Window
	destroyWindow(m_wnd);
	m_wnd = nullptr;
	puts("Quit the App");
}

bool App::IsRunning()
{
	return (glfwWindowShouldClose(m_wnd) == false);
}

void App::CreateAttachments()
{
	// ATTACHMENTS
	// Render Pass Color Attachment
	m_renderPassColorAttach = createRenderPassColorAttach();
	puts("Created the Attachments");
}

void App::CreateDescriptors()
{
	// DESCRIPTORS
	// Command Encoder Descriptor
	m_encoderDesc = createEncoderDesc();
	// Render Pass Descriptor
	m_renderPassDesc = createRenderPassDesc(m_renderPassColorAttach);
	// Command Buffer Descriptor
	m_cmdBufferDesc = createCmdBufferDesc();
	puts("Created the Descriptors");
}

void App::LoadShader()
{
	m_shaderMod = createShaderMod(m_gpuEnv.dev);
}

void App::CreateRenderPipeline()
{
	// Define Pipeline Layout:
	WGPURenderPipelineDescriptor pipelineDesc{};
	pipelineDesc.nextInChain = nullptr;
	
    // Create a pipeline layout to define the input structure.
	// We do not use any vertex buffer for this first simplistic example
	pipelineDesc.vertex.bufferCount = 0;
	pipelineDesc.vertex.buffers = nullptr;
	
	// Set Up Shader Stages:
    // Specify the vertex and fragment shader stages.
	// NB: We define the 'shaderModule' in the second part of this chapter.
	// Here we tell that the programmable vertex shader stage is described
	// by the function called 'vs_main' in that module.
	pipelineDesc.vertex.module = m_shaderMod;
	pipelineDesc.vertex.entryPoint = "vs_main";
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;
	
	// Each sequence of 3 vertices is considered as a triangle
	pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
	
	// We'll see later how to specify the order in which vertices should be
	// connected. When not specified, vertices are considered sequentially.
	pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
	
	// The face orientation is defined by assuming that when looking
	// from the front of the face, its corner vertices are enumerated
	// in the counter-clockwise (CCW) order.
	pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
	
	// But the face orientation does not matter much because we do not
	// cull (i.e. "hide") the faces pointing away from us (which is often
	// used for optimization).
	pipelineDesc.primitive.cullMode = WGPUCullMode_None;

	// We tell that the programmable fragment shader stage is described
	// by the function called 'fs_main' in the shader module.
	WGPUFragmentState fragmentState = {};
	fragmentState.module = m_shaderMod;
	fragmentState.entryPoint = "fs_main";
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;

	// Configure Pipeline States:
    // Configure vertex input, rasterization, and color blending states.
	WGPUBlendState blendState = {};
	blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	blendState.color.operation = WGPUBlendOperation_Add;
	blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	blendState.alpha.dstFactor = WGPUBlendFactor_One;
	blendState.alpha.operation = WGPUBlendOperation_Add;
	
	WGPUColorTargetState colorTarget = {};
	colorTarget.format = WGPUTextureFormat_BGRA8Unorm;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.
	
	// We have only one target because our render pass has only one output color
	// attachment.
	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;
	pipelineDesc.fragment = &fragmentState;

	// We do not use stencil/depth testing for now
	pipelineDesc.depthStencil = nullptr;

	// Samples per pixel
	pipelineDesc.multisample.count = 1;

	// Default value for the mask, meaning "all bits on"
	pipelineDesc.multisample.mask = ~0u;

	// Default value as well (irrelevant for count = 1 anyways)
	pipelineDesc.multisample.alphaToCoverageEnabled = false;
	pipelineDesc.layout = nullptr;

	// Create Render Pipeline:
    // Use the device to create the render pipeline.
	m_gpuEnv.pipeline = wgpuDeviceCreateRenderPipeline(m_gpuEnv.dev, &pipelineDesc);

	// Use Pipeline in Command Buffers:
    // Integrate the pipeline in command buffers to execute rendering commands.
}

void App::EventLoop()
{
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	
	// Poll WebGPU Events
	#if defined(WEBGPU_BACKEND_DAWN)
	wgpuDeviceTick(m_gpuenv.dev);
	#elif defined(WEBGPU_BACKEND_WGPU)
	wgpuDevicePoll(m_gpuEnv.dev, false, nullptr);
	#endif
}

void App::RenderPass()
{
	// {{Create a Command Encoder}}
	WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_gpuEnv.dev, &m_encoderDesc);
	
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_renderPassDesc);
	
	// {{Draw}}
	// Select which render pipeline to use
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	// Draw 1 instance of a 3-vertices shape
	wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);

	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
	// {{Finish encoding the Command}}
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &m_cmdBufferDesc);
	// {{Release the Command Encoder}}
	wgpuCommandEncoderRelease(encoder);
	
	// {{Submit the Command}}
	//puts("Submitting command...");
	wgpuQueueSubmit(m_gpuEnv.queue, 1, &command);
	wgpuCommandBufferRelease(command);
	//puts("Command submitted.");
}

void App::Display()
{
	wgpuTextureViewRelease(m_targetView);
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(m_gpuEnv.surf);
	#endif
}

void App::Run()
{
	// MAIN LOOP
	while (this->IsRunning())
	{
		// Run Event Loop
		this->EventLoop();
		
		// Get the next Target Texture View
		m_targetView = getNextTextureView(m_gpuEnv.surf);
		// Set the Target Texture View to Color Attachment
		m_renderPassColorAttach.view = m_targetView;
		
		// Submit a Render Pass
		this->RenderPass();
		
		// At the End of Frame: display
		this->Display();
	}
}
