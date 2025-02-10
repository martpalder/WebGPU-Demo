#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
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
	
	// Methods
	void CreateAttachments();
	void CreateDescriptors();
	void EventLoop();
	void RenderPass();

public:
	// Constructor/Destructor
	App();
	~App();
	
	// Methods
	void Run();
};

#endif	// APP_HPP_INCLUDED
