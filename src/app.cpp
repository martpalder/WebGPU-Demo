#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./layout.hpp"
#include "./pipeline.hpp"
#include "./buffer.hpp"
#include "./texture.hpp"
#include "./view.hpp"
#include "./sampler.hpp"
#include "./texture_loader.hpp"
#include "./mymath.h"
#include "./stdafx.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define PLAYER_OBJ "triangle3d_color.obj"
#define SHADER_NAME "basic3d_color.wgsl"

App::App(int w, int h, const char* title)
{
	// Set Default Values
	m_wnd = nullptr;
	m_bindGroupLayout = nullptr;
	m_pCam = nullptr;
	m_pPlayer = nullptr;
	
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
	m_w = w;
	m_h = h;
	m_wnd = createWindow(w, h, title);
	// Set the Mouse Mode
	glfwSetInputMode(m_wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	// Initialize WebGPU
	m_gpuEnv = initGPUEnv(m_wnd);
	// Initialize Input
	m_input.Init(m_wnd);
	
	// Load Data
	this->LoadData();
	
	// Setup Actors
	this->SetupActors();
	
	// Create the Render Pipeline
	this->CreatePipeline();
	//createSampler(m_gpuEnv.dev);
	puts("Initialized the App");
}

void App::Quit()
{
	// Release the Depth View
	wgpuTextureViewRelease(m_depthView);
	printRelease("Depth View");
	// Release the Depth Texture
	wgpuTextureRelease(m_depthTexture);
	printRelease("Depth Texture");
	
	// Release the Target View
	wgpuTextureViewRelease(m_targetView);
	printRelease("Target View");
	
	// Release the Layouts
	if (m_bindGroupLayout != nullptr)
	{
		wgpuBindGroupLayoutRelease(m_bindGroupLayout);
		m_bindGroupLayout = nullptr;
		printRelease("Bind Group Layout");
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

void App::LoadData()
{
	// Load a Shader
	m_shaderMgr.Load(m_gpuEnv.dev, SHADER_NAME);
	
	// Load Meshes
	m_meshMgr.Load(m_gpuEnv, PLAYER_OBJ);
	m_meshMgr.Load(m_gpuEnv, "ground.obj");
	
	// TODO: Load Textures
	//m_texMgr.Load(m_gpuEnv, "char_base.png");
}

void App::CreatePipeline()
{
	// Create a Depth Texture and View
	WGPUTextureFormat depthFormat = WGPUTextureFormat_Depth16Unorm;
	m_depthTexture = createDepthTexture(m_gpuEnv.dev, m_w, m_h, depthFormat);
	m_depthView = createDepthView(m_depthTexture);
	
	// Create the Attachments
	m_attach.colorAttach = createColorAttach(0.1f, 0.1f, 0.1f);
	m_attach.depthStencilAttach = createDepthStencilAttach(m_gpuEnv.dev, m_depthView);
	puts("Created the Attachments");
	
	// Create the Descriptors
	bool bDepthStencil = true;
	m_desc = createDescriptors(m_attach, bDepthStencil);
	
	// Create the Pipeline States
    States states = createStates(m_shaderMgr.Get(SHADER_NAME), depthFormat);
    // Create the Pipeline Descriptor
	WGPURenderPipelineDescriptor pipelineDesc = createRenderPipelineDesc(states, bDepthStencil);
	
	// Create the Layouts
	m_bindingLayout = createLayoutBinding(sizeof(mat4x4));
	m_bindGroupLayout = createLayoutBindGroup(m_gpuEnv.dev, &m_bindingLayout);
	// Create the Bind Groups
	m_world.CreateBindGroups(m_gpuEnv, m_bindGroupLayout);
    
    // PIPELINE LAYOUT
	// Assign the PipelineLayout to the RenderPipelineDescriptor's layout field
	pipelineDesc.layout = createLayoutPipeline(m_gpuEnv.dev, &m_bindGroupLayout);
	puts("Assigned the PipelineLayout to the RenderPipelineDescriptor");
	
	// Create the Render Pipeline
	m_gpuEnv.pipeline = createRenderPipeline(m_gpuEnv, pipelineDesc);
}

void App::SetupActors()
{	
	// Add the Actors with Meshes
	// Player
	m_pPlayer = m_world.AddActor(m_gpuEnv, 0.0f, 0.0f, 0.0f, "Player");
	m_pPlayer->SetMesh(m_meshMgr.Get(PLAYER_OBJ));
	Actor* pGround = m_world.AddActor(m_gpuEnv, 0.0f, -0.5f, 0.0f, "Ground");
	pGround->SetMesh(m_meshMgr.Get("ground.obj"));
	
	// Get and attach the Camera
	m_pCam = m_world.GetCam();
	//m_pCam->SetParent(m_pPlayer);
}

void App::Cls()
{
	// Get the next Target Texture View
	getNextTargetView(m_gpuEnv.surf, &m_targetView);
	// Assign the Target Texture View to Color Attachment
	m_attach.colorAttach.view = m_targetView;
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
	renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_desc.renderPassDesc);
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	
	// {{Draw}}
	m_world.Draw(renderPass);
	
	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void App::Update()
{
	// Get Move Input
	float h = m_input.GetAxis(0);
	float v = m_input.GetAxis(1);
	
	// If there's Move Input and Player
	if ((h || v) && m_pPlayer != nullptr)
	{
		// Calculate the Move Direction
		vec2 moveDir;
		calcMoveDir(h, v, 0.0f, moveDir);
		// Move the Player
		m_pPlayer->MoveAndCollide(moveDir);
		
		// Get the Camera Yaw
		/*float yaw = m_pCam->GetYaw();
		// Rotate the Player
		if (v < 0.0f){ yaw += M_PI; };
		m_pPlayer->SetYaw(yaw - h * HALF_PI);*/
	}
	
	// If there's Camera
	/*if (m_pCam != nullptr)
	{
		// Get the Mouse Position
		vec2 mDelta;
		copyVec2(mDelta, m_input.GetMDelta());
		// Rotate the Camera
		m_pCam->Orbit(mDelta);
		// Reset Input
		m_input.Reset();
	}*/
	
	// {{Update}}
	m_world.Update(m_gpuEnv.queue);
}

void App::Draw()
{
	// Clear the Screen
	this->Cls();
	
	// {{Create a Command Encoder}}
	WGPUCommandEncoder encoder;
	encoder = wgpuDeviceCreateCommandEncoder(m_gpuEnv.dev, &m_desc.encoderDesc);
	
	// {{Do a Render Pass}}
	this->RenderPass(encoder);
	
	// {{Finish encoding the Command}}
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &m_desc.cmdBufferDesc);
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
