#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./input.hpp"
#include "./desc.hpp"
#include "./attach.hpp"
#include "./shader.hpp"
#include "./actor.hpp"

#include <GLFW/glfw3.h>
#include <linmath.h>

class App
{
private:
	// Main Members
	GPUEnv m_gpuEnv;
	Input m_input;
	GLFWwindow* m_wnd;
	
	// Descriptors
	Descriptors m_descriptors = {};
	// Attachments
	Attachments m_attachments = {};
	// Layouts
	WGPUBindGroupLayoutEntry m_bindingLayout;
	WGPUBindGroupLayout m_bindGroupLayout;
	
	// RESOURCES
	// Shaders
	Shader m_shader;
	// Actors
	Actor m_player, m_terrain;
	
	// Getters
	WGPUBool IsRunning();
	
	// Setters
	void SetDefaults();
	
	// Init/Quit Methods
	void Init(int w, int h, const char* title);
	void Quit();
	
	// Creation Methods
	void CreateAttachments();
	void CreatePipeline();
	
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
