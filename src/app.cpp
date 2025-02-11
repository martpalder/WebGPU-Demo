#include "./app.hpp"

#include "./window.hpp"
#include "./desc.hpp"
#include "./config.hpp"
#include "./attach.hpp"
#include "./texture.hpp"
#include "./pipeline.hpp"

#include <cstdio>

const char* SHADER_CODE = R"(
	@vertex
	fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f
	{
		var p = vec2f(0.0, 0.0);
		if (in_vertex_index == 0u) {
			p = vec2f(-0.5, -0.5);
		} else if (in_vertex_index == 1u) {
			p = vec2f(0.5, -0.5);
		} else {
			p = vec2f(0.0, 0.5);
		}
		return vec4f(p, 0.0, 1.0);
	}

	@fragment
	fn fs_main() -> @location(0) vec4f
	{
		return vec4f(0.0, 0.4, 1.0, 1.0);
	}
)";

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

	// Load a Shader
	m_shader.Load(m_gpuEnv.dev, "./shaders/triangle.wgsl");
	
	// Create the Render Pipeline
	m_gpuEnv.pipeline = createRenderPipeline(m_gpuEnv.dev, m_shader.GetShaderMod());
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
