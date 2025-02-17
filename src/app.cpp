#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./layout.hpp"
#include "./pipeline.hpp"
#include "./buffer.hpp"
#include "./texture.hpp"
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
	// Initialize Input
	m_input.Init(m_wnd);
	
	// Create the Attachments and Descriptors
	this->CreateAttachments();
	m_descriptors = createDescriptors(m_attachments, true);

	// Load a Shader
	m_shader.Load(m_gpuEnv.dev, "basic3d_color.wgsl");
	// Load Meshes
	m_terrain.LoadMesh(m_gpuEnv, "plane.obj");
	m_player.LoadMesh(m_gpuEnv, "cube.obj");
	
	// Create the Render Pipeline
	this->CreatePipeline();
	puts("Initialized the App");
	
	// Set Terrain
	m_terrain.Translate(0.0f, -2.0f, 0.0f);
}

void App::Quit()
{
	if (m_bindGroupLayout != nullptr)
	{
		wgpuBindGroupLayoutRelease(m_bindGroupLayout);
		m_bindGroupLayout = nullptr;
		puts("Released the Bind Group Layout");
	}
	
	// Quit WebGPU
	quitGPUEnv(m_gpuEnv);
	// Destroy the Window
	destroyWindow(m_wnd);
	m_wnd = nullptr;
	puts("Quit the App");
}

WGPUBool App::IsRunning()
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
	// Create a Depth Texture
	WGPUTexture depthTexture = createDepthTexture(m_gpuEnv.dev, 800, 600);
	
	// ATTACHMENTS
	// Render Pass Color Attachment
	m_attachments.colorAttach = createColorAttach(0.1f, 0.1f, 0.1f);
	m_attachments.depthStencilAttach = createDepthStencilAttach(m_gpuEnv.dev, depthTexture);
	// TODO: Depth Stencil Attachment
	puts("Created the Attachments");
}

void App::CreatePipeline()
{
	// Create Bind Group Layout
	m_bindingLayout = createLayoutBinding(sizeof(mat4x4));
	m_bindGroupLayout = createLayoutBindGroup(m_gpuEnv.dev, &m_bindingLayout);
	
	// Create the Bind Groups
	m_terrain.CreateBindGroup(m_gpuEnv.dev, m_bindGroupLayout);
	m_player.CreateBindGroup(m_gpuEnv.dev, m_bindGroupLayout);
	
	// Create the Render Pipeline
	m_gpuEnv.pipeline = createRenderPipeline(
		m_gpuEnv.dev,
		m_shader.GetShaderMod(),
		&m_bindGroupLayout
	);
}

void App::Cls()
{
	// Get the next Target Texture View
	getNextTargetView(m_gpuEnv.surf, &m_gpuEnv.targetView);
	// Assign the Target Texture View to Color Attachment
	m_attachments.colorAttach.view = m_gpuEnv.targetView;
}

void App::Flip()
{
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(m_gpuEnv.surf);
	#endif
}

void App::EventLoop()
{
	#ifndef __EMSCRIPTEN__
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	#endif
}

void App::RenderPass(const WGPUCommandEncoder& encoder)
{
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass;
	renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_descriptors.renderPassDesc);
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	
	// {{Draw}}
	m_terrain.Draw(renderPass);
	m_player.Draw(renderPass);
	
	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void App::Update()
{
	// {{Update}}
	m_player.Update(m_gpuEnv.queue);
	m_terrain.Update(m_gpuEnv.queue);
	
	// Get Move Input
	float h = m_input.GetAxis(0);
	float v = m_input.GetAxis(1);
	
	// If there's Move Input
	if (h != 0.0f || v != 0.0f)
	{
		// Move tbe Player
		m_player.Translate(h, 0.0f, -v);
	}
	//m_terrain.RotateX(0.1f);
}

void App::Draw()
{
	// Clear the Screen
	this->Cls();
	
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
	
	// Flip the Display Buffer
	this->Flip();
}

void App::MainLoopGLFW()
{
	// Main Loop for GLFW
	while (this->IsRunning())
	{
		// Run the Event Loop
		this->EventLoop();	
		// Update
		this->Update();
		// Draw
		this->Draw();
	}
}

void App::MainLoopEM()
{
	#ifdef __EMSCRIPTEN__
	// Equivalent of the Main Loop when using Emscripten:
	auto callback = [](void* arg)
	{
		// Get the App
		App* pApp = reinterpret_cast<App*>(arg);
		// Run the Event Loop
		pApp->EventLoop();	
		// Update
		pApp->Update();
		// Draw
		pApp->Draw();
	};
	
	// Set the Main Loop
	emscripten_set_main_loop_arg(callback, this, 0, true);
	#endif
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
