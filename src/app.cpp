#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./desc.hpp"
#include "./config.hpp"
#include "./bind.hpp"
#include "./pipeline.hpp"
#include "./texture.hpp"

#include <cstdio>
#include <emscripten/emscripten.h>

App::App(int w, int h, const char* title)
{
	// INITIALIZATION
	this->Init(w, h, title);
}

App::~App()
{
	// QUIT
	this->Quit();
}

void App::Init(int w, int h, const char* title)
{
	#ifndef __EMSCRIPTEN__
	// Create the window
	m_wnd = createWindow(w, h, title);
	if (m_wnd == nullptr){ this->~App(); }
	#endif
	
	// Initialize WebGPU
	m_gpuEnv = initGPUEnv(m_wnd);
	
	// Configure the Surface
	WGPUSurfaceConfiguration config = createSurfConfig(w, h, m_gpuEnv.dev);
	wgpuSurfaceConfigure(m_gpuEnv.surf, &config);
	
	// Create Attachments, Descriptors and Bindings
	this->CreateAttachments();
	this->CreateDescriptors();
	//this->CreateBindings();

	// Load a Shader
	m_shader.Load(m_gpuEnv.dev, "basic3d.wgsl");
	// Create the Render Pipeline
	this->CreatePipeline();
	
	// Create a Projection Matrix
	this->CreateProjection();
	
	// Load a Mesh
	m_mesh.Load(m_gpuEnv.dev, m_gpuEnv.queue);
	puts("Initialized the App");
}

void App::Quit()
{
	// Release the Projection Buffer
	wgpuBufferRelease(m_projBuffer);
	m_projBuffer = nullptr;
	
	// Quit WebGPU
	quitGPUEnv(m_gpuEnv);
	// Destroy the Window
	destroyWindow(m_wnd);
	m_wnd = nullptr;
	puts("Quit the App");
}

bool App::IsRunning()
{
	#ifndef __EMSCRIPTEN__
	return (glfwWindowShouldClose(m_wnd) == false);
	#else
	return true;
	#endif
}

void App::SetBindGroup(const WGPURenderPassEncoder& renderPass)
{
	if (m_bindGroup != nullptr)
	{
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bindGroup, 0, nullptr);
	}
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

void App::CreateBindings()
{
	m_bindGroup = createBindGroup(m_gpuEnv.dev, sizeof(mat4x4), m_projBuffer);
}

void App::CreatePipeline()
{
	m_gpuEnv.pipeline = createRenderPipeline(m_gpuEnv.dev, m_shader.GetShaderMod(), (m_bindGroup == nullptr));
}

void App::CreateProjection()
{
	float fov = 60.0f;
	float aspect = 4 / 3.0f;
	float n = 0.1f;
	float f = 100.0f;
	mat4x4_perspective(m_proj, fov, aspect, n, f);
}

void App::EventLoop()
{
	#ifndef __EMSCRIPTEN__
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	#endif
	
	// Poll WebGPU Events
	#if defined(WEBGPU_BACKEND_DAWN)
	wgpuDeviceTick(m_gpuenv.dev);
	#elif defined(WEBGPU_BACKEND_WGPU)
	wgpuDevicePoll(m_gpuEnv.dev, false, nullptr);
	#endif
}

void App::Cls()
{
	// Get the next Target Texture View
	m_targetView = getNextTextureView(m_gpuEnv.surf);
	// Set the Target Texture View to Color Attachment
	m_renderPassColorAttach.view = m_targetView;
}

void App::Flip()
{
	wgpuTextureViewRelease(m_targetView);
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(m_gpuEnv.surf);
	#endif
}

void App::RenderPass(const WGPUCommandEncoder& encoder)
{
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_renderPassDesc);
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	
	// {{Set the binding group here!}}
	//this->SetBindGroup(renderPass);
	
	// {{Draw}}
	m_mesh.Draw(renderPass);

	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void App::Draw()
{
	// {{Create a Command Encoder}}
	WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_gpuEnv.dev, &m_encoderDesc);
	
	// {{Do a Render Pass}}
	this->RenderPass(encoder);
	
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

void App::MainLoopGLFW()
{
	// Main Loop for GLFW
	while (this->IsRunning())
	{
		// Run the Event Loop
		this->EventLoop();
		
		// Clear
		this->Cls();
		// Draw
		this->Draw();
		// Flip
		this->Flip();
	}
}

void App::MainLoopEM()
{
	// Equivalent of the Main Loop when using Emscripten:
	auto callback = [](void *arg)
	{
		App* pApp = reinterpret_cast<App*>(arg);
		// Run the Event Loop
		pApp->EventLoop();
		
		// Clear
		pApp->Cls();
		// Draw
		pApp->Draw();
		// Flip
		pApp->Flip();
	};
	
	// Set the Main Loop
	//emscripten_set_main_loop_arg(callback, this, 0, true);
}

void App::Run()
{
	// MAIN LOOP
	#ifdef __EMSCRIPTEN__
	this->MainLoopEM();
	#else
	this->MainLoopGLFW();
	#endif
}
