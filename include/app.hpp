#pragma once
#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./shader.hpp"
#include "./mesh.hpp"
#include "./bind.hpp"

#include <GLFW/glfw3.h>
#include <linmath/linmath.h>

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
	// Bindings
	Bind m_bind;
	
	// Projection Matrix and Buffer
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
	void SetDefaults();
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
