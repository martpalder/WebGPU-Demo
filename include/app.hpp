#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./shader.hpp"
#include <GLFW/glfw3.h>

class App
{
private:
	// Main Members
	GPUEnv m_gpuEnv;
	GLFWwindow* m_wnd;
	// Descriptors
	WGPUCommandEncoderDescriptor m_encoderDesc;
	WGPURenderPassDescriptor m_renderPassDesc;
	WGPUCommandBufferDescriptor m_cmdBufferDesc;
	// Attachments
	WGPURenderPassColorAttachment m_renderPassColorAttach;
	// Target View
	WGPUTextureView m_targetView;
	// Shaders
	Shader m_shader;
	
	// Getters
	bool IsRunning();
	
	// Methods
	void CreateAttachments();
	void CreateDescriptors();
	void EventLoop();
	void RenderPass();
	void Display();

public:
	// Constructor/Destructor
	App();
	~App();
	
	// Methods
	void Run();
};

#endif	// APP_HPP_INCLUDED
