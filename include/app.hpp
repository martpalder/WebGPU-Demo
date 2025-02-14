#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./shader.hpp"
#include "./actor.hpp"

#include <GLFW/glfw3.h>
#include <linmath.h>

typedef struct {
	WGPUCommandEncoderDescriptor encoderDesc;
	WGPURenderPassDescriptor renderPassDesc;
	WGPUCommandBufferDescriptor cmdBufferDesc;
} Descriptors;

class App
{
private:
	// Main Members
	GPUEnv m_gpuEnv;
	GLFWwindow* m_wnd;
	
	// Descriptors
	Descriptors m_descriptors;
	// Attachments
	WGPURenderPassColorAttachment m_renderPassColorAttach;
	
	// RESOURCES
	// Shaders
	Shader m_shader;
	// Actors
	Actor m_player;
	
	// Getters
	bool IsRunning();
	
	// Setters
	void SetDefaults();
	
	// Init/Quit Methods
	void Init(int w, int h, const char* title);
	void Quit();
	
	// Creation Methods
	void CreateAttachments();
	void CreateDescriptors();
	void CreatePipeline();
	
	// Main Methods
	void EventLoop();
	void Cls();
	void Flip();
	void RenderPass(const WGPUCommandEncoder& encoder);
	void Draw();
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
