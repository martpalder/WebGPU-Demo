#include "./app.hpp"
#include "./window.hpp"
#include "./attach.hpp"
#include "./view.hpp"
#include "./texture_loader.hpp"
#include "./mymath.h"
#include "./stdafx.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define SHADER_NAME "basic3d_color.wgsl"
#define PLAYER_OBJ "cube.obj"
#define GROUND_OBJ "ground.obj"

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
	
	// Initialize the World
	m_world.Init(m_gpuEnv.dev, m_shaderMgr.Get(SHADER_NAME), m_w, m_h);
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
	m_meshMgr.Load(m_gpuEnv, GROUND_OBJ);
	
	// TODO: Load Textures
	//m_texMgr.Load(m_gpuEnv, "char_base.png");
}

void App::SetupActors()
{
	// Create the Layouts
	m_bindingLayout = createLayoutBinding(sizeof(mat4x4));
	m_bindGroupLayout = createLayoutBindGroup(m_gpuEnv.dev, &m_bindingLayout);
	
	// Setup the Actors
	// Player
	m_pPlayer = m_world.AddActor(m_gpuEnv, 0.0f, 0.0f, 0.0f, "Player");
	m_pPlayer->SetMesh(m_meshMgr.Get(PLAYER_OBJ));
	m_pPlayer->CreateBindGroup(m_gpuEnv.dev, m_bindGroupLayout);
	// Ground
	Actor* pGround = m_world.AddActor(m_gpuEnv, 0.0f, 0.0f, 0.0f, "Ground");
	pGround->SetMesh(m_meshMgr.Get(GROUND_OBJ));
	pGround->CreateBindGroup(m_gpuEnv.dev, m_bindGroupLayout);
	
	// Get and attach the Camera
	m_pCam = m_world.GetCam();
	//m_pCam->SetParent(m_pPlayer);
}

void App::EventLoop()
{
	#ifndef __EMSCRIPTEN__
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	#endif
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

void App::Render()
{
	m_world.Cls(m_gpuEnv.surf);
	m_world.Draw(m_gpuEnv.dev, m_gpuEnv.queue);
	m_world.Flip(m_gpuEnv.surf);
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
		// Render
		this->Render();
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
		// Render
		pApp->Render();
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
