#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./shader.hpp"
#include "./mesh.hpp"

#include <GLFW/glfw3.h>
#include <linmath/linmath.h>

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
	// Binding Group
	WGPUBindGroupLayout m_bindGroupLayout;
	WGPUBindGroup m_bindGroup;
	// Matrices
	mat4x4 m_proj;
	WGPUBuffer m_projBuffer;
	
	// RESOURCES
	// Shaders
	Shader m_shader;
	// Meshes
	Mesh m_mesh;
	
	// Getters
	bool IsRunning();
	
	// Setters
	void SetBindGroup(const WGPURenderPassEncoder& renderPass);
	
	// Init/Quit Methods
	void Init(int w, int h, const char* title);
	void Quit();
	
	// Creation Methods
	void CreateAttachments();
	void CreateDescriptors();
	void CreateBindings();
	void CreatePipeline();
	void CreateProjection();
	
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
