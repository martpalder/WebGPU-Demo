#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./desc.hpp"
#include "./layout.hpp"
#include "./pipeline.hpp"
#include "./buffer.hpp"
#include "./view.hpp"
#include "./callback.hpp"

#include <cstdio>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

App::App(int w, int h, const char* title)
{
	// Set Default Values
	this->SetDefaults();
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
	// Create the window
	m_wnd = createWindow(w, h, title);
	
	// Initialize WebGPU
	m_gpuEnv = initGPUEnv(m_wnd);
	
	// Create the Attachments and Descriptors
	this->CreateAttachments();
	this->CreateDescriptors();

	// Load a Shader
	m_shader.Load(m_gpuEnv.dev, "basic3d_color.wgsl");
	// Load a Mesh
	m_player.CreateBindings(m_gpuEnv);
	m_player.LoadMesh(m_gpuEnv);
	
	// Create the Bindings
	//m_mesh.CreateBindings(m_gpuEnv.dev, m_gpuEnv.queue);
	// Create the Render Pipeline
	this->CreatePipeline();
	puts("Initialized the App");
}

void App::Quit()
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
	#ifndef __EMSCRIPTEN__
	return (glfwWindowShouldClose(m_wnd) == false);
	#else
	return true;
	#endif
}

void App::SetDefaults()
{
	// Set Default Values
	// Window
	m_wnd = nullptr;
}

void App::CreateAttachments()
{
	// ATTACHMENTS
	// Render Pass Color Attachment
	m_renderPassColorAttach = createRenderPassColorAttach(0.1f, 0.1f, 0.1f);
	puts("Created the Attachments");
}

void App::CreateDescriptors()
{
	// DESCRIPTORS
	// Command Encoder Descriptor
	m_descriptors.encoderDesc = createEncoderDesc();
	// Render Pass Descriptor
	m_descriptors.renderPassDesc = createRenderPassDesc(m_renderPassColorAttach);
	// Command Buffer Descriptor
	m_descriptors.cmdBufferDesc = createCmdBufferDesc();
	puts("Created the Descriptors");
}

void App::CreatePipeline()
{
	// Create the Render Pipeline
	m_gpuEnv.pipeline = createRenderPipeline(
		m_gpuEnv.dev,
		m_shader.GetShaderMod(),
		m_player.GetBindGroupLayout()
	);
}

void App::EventLoop()
{
	#ifndef __EMSCRIPTEN__
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	#endif
}

void App::Cls()
{
	// Get the next Target Texture View
	getNextTargetView(m_gpuEnv.surf, &m_gpuEnv.targetView);
	// Assign the Target Texture View to Color Attachment
	m_renderPassColorAttach.view = m_gpuEnv.targetView;
}

void App::Flip()
{
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(m_gpuEnv.surf);
	#endif
}

void App::RenderPass(const WGPUCommandEncoder& encoder)
{
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass;
	renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_descriptors.renderPassDesc);
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	
	// {{Update}}
	m_player.Update(m_gpuEnv.queue, renderPass);
	
	// {{Draw}}
	m_player.Draw(renderPass);

	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void App::Draw()
{
	// {{Create a Command Encoder}}
	WGPUCommandEncoder encoder;
	encoder = wgpuDeviceCreateCommandEncoder(m_gpuEnv.dev, &m_descriptors.encoderDesc);
	
	// {{Do a Render Pass}}
	this->RenderPass(encoder);
	
	// {{Finish encoding the Command}}
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &m_descriptors.cmdBufferDesc);
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
