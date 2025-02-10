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
	m_cmdBufferDesc = {};
	m_cmdBufferDesc.nextInChain = nullptr;
	m_cmdBufferDesc.label = "Command buffer";
	puts("Created the Descriptors");
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
	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
	// {{Finish encoding the Command}}
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &m_cmdBufferDesc);
	// {{Release the Command Encoder}}
	wgpuCommandEncoderRelease(encoder);
	
	// {{Submit the Command}}
	puts("Submitting command...");
	wgpuQueueSubmit(m_gpuEnv.queue, 1, &command);
	wgpuCommandBufferRelease(command);
	puts("Command submitted.");
}

void App::Run()
{
	// MAIN LOOP
	while (!glfwWindowShouldClose(m_wnd))
	{
		// Run Event Loop
		this->EventLoop();
		
		// Get the next Target Texture View
		WGPUTextureView targetView = getNextTextureView(m_gpuEnv.surf);
		m_renderPassColorAttach.view = targetView;
		
		// Submit a Render Pass
		this->RenderPass();
		
		// At the End of Frame
		wgpuTextureViewRelease(targetView);
		#ifndef __EMSCRIPTEN__
		wgpuSurfacePresent(m_gpuEnv.surf);
		#endif
	}
}
