#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./input.hpp"
#include "./attach.hpp"
#include "./desc.hpp"
#include "./world.hpp"
#include "./mesh_manager.hpp"
#include "./texture_manager.hpp"
#include "./shader_manager.hpp"

#include <GLFW/glfw3.h>
#include "./stdafx.h"

class App
{
private:
	// Main Members
	int m_w, m_h;
	GPUEnv m_gpuEnv;
	Input m_input;
	GLFWwindow* m_wnd;
	
	/*WGPUTexture depthTexture;
	WGPUTextureView depthView;*/
	// Attachments and Descriptors
	Attachments m_attach;
	Descriptors m_desc;
	
	// Layouts
	WGPUBindGroupLayoutEntry m_bindingLayout;
	WGPUBindGroupLayout m_bindGroupLayout;
	// Target Texture View
	WGPUTextureView m_targetView;
	
	// World
	World m_world;
	// Camera
	Camera* m_pCam;
	// Actors
	Actor* m_pPlayer;
	
	// RESOURCES
	// Managers
	MeshManager m_meshMgr;
	TextureManager m_texMgr;
	ShaderManager m_shaderMgr;
	
	// Getters
	WGPUBool IsRunning();
	
	// Setters
	void SetDefaults();
	
	// Init/Quit Methods
	void Init(int w, int h, const char* title);
	void Quit();
	
	// Create Methods
	void CreateAttachments();
	void CreatePipeline();
	
	// Load Methods
	void LoadData();
	
	// Setup Mehods
	void SetupActors();
	
	// Main Methods
	void Cls();
	void Flip();
	void EventLoop();
	void RenderPass(const WGPUCommandEncoder& encoder);
	void Update();
	void Draw();
	
	// Main Loops
	void MainLoopGLFW();
	void MainLoopEM();

public:
	// Constructor/Destructor
	App(int w, int h, const char* title);
	~App();
	
	// Methods
	void Run();
};

#endif	// APP_HPP_INCLUDED
