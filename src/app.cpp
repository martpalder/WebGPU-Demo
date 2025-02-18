#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./layout.hpp"
#include "./pipeline.hpp"
#include "./buffer.hpp"
#include "./texture.hpp"
#include "./view.hpp"
#include "./callback.hpp"
#include "./mymath.h"
#include "./stdafx.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

App::App(int w, int h, const char* title)
{
	// Set Default Values
	m_wnd = nullptr;
	m_pCam = nullptr;
	m_pPlayer = nullptr;
	mat4x4_identity(m_p);
	
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
	// Setup Actors
	this->SetupActors();
	
	// Create the Render Pipeline
	this->CreatePipeline();
	
	// Setup Projection Matrix
	mat4x4_perspective(m_p, 90.0f, 4 / 3.0f, 0.1f, 100.0f);
	puts("Initialized the App");
}

void App::Quit()
{
	// Release the Layouts
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

void App::CreateAttachments()
{
	// Create a Depth Texture
	WGPUTexture depthTexture = createDepthTexture(m_gpuEnv.dev, 800, 600);
	
	// Create Attachments
	m_attachments.colorAttach = createColorAttach(0.1f, 0.1f, 0.1f);
	m_attachments.depthStencilAttach = createDepthStencilAttach(m_gpuEnv.dev, depthTexture);
	puts("Created the Attachments");
}

void App::CreatePipeline()
{
	// Create the Layouts
	m_bindingLayout = createLayoutBinding(sizeof(mat4x4));
	m_bindGroupLayout = createLayoutBindGroup(m_gpuEnv.dev, &m_bindingLayout);
	
	// Create the Bind Groups
	m_world.CreateBindGroups(m_gpuEnv, m_bindGroupLayout);
	
	// Create the Render Pipeline
	m_gpuEnv.pipeline = createRenderPipeline(
		m_gpuEnv.dev,
		m_shader.GetShaderMod(),
		&m_bindGroupLayout
	);
}

void App::SetupActors()
{
	// Load Meshes
	Mesh* pCubeMesh = m_meshMgr.Load(m_gpuEnv, "cube.obj");
	Mesh* pGroundMesh = m_meshMgr.Load(m_gpuEnv, "ground.obj");
	Mesh* pPlaneMesh = m_meshMgr.Load(m_gpuEnv, "plane.obj");
	
	// Get the Camera
	m_pCam = m_world.GetCam();
	
	// Add the Actors with Meshes
	// Player
	m_pPlayer = m_world.AddActor(0.0f, 0.0f, 0.0f, "Player");
	m_pPlayer->SetMesh(pCubeMesh);
	m_pCam->SetParent(m_pPlayer);
	// Ground
	Actor* pGround = m_world.AddActor(0.0f, -0.5f, 0.0f, "Ground");
	pGround->SetMesh(pGroundMesh);
	// Billboard
	Actor* pBill = m_world.AddActor(0.0f, 0.5f, -2.0f, "Billboard");
	pBill->SetMesh(pPlaneMesh);
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
	
	// If there's Move Input
	if ((h || v) && m_pPlayer != nullptr)
	{
		// Calculate the Move Direction
		vec2 moveDir;
		calcMoveDir(h, v, m_pCam->GetYaw(), moveDir);
		// Move the Player
		m_pPlayer->MoveAndCollide(moveDir);
	}
	
	// Get the Mouse Position
	vec2 mDelta;
	setVec2(mDelta, m_input.GetMDelta());
	// Rotate the Camera
	m_pCam->Orbit(mDelta);
	// Reset Input
	m_input.Reset();
	
	// {{Update}}
	m_world.Update(m_gpuEnv.queue, m_p);
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
