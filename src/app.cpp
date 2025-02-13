#include "./app.hpp"
#include "./window.hpp"
#include "./config.hpp"
#include "./attach.hpp"
#include "./desc.hpp"
#include "./layout.hpp"
#include "./bind.hpp"
#include "./pipeline.hpp"
#include "./buffer.hpp"
#include "./view.hpp"

#include <cstdio>
#include <emscripten/emscripten.h>

App::App(int w, int h, const char* title)
{
	// Set Default Values
	this->SetDefaults();
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
	if (m_wnd == nullptr){ this->~App(); }
	
	// Initialize WebGPU
	m_gpuEnv = initGPUEnv(m_wnd);
	
	#ifndef __EMSCRIPTEN__
	// Configure the Surface
	WGPUSurfaceConfiguration config = createSurfConfig(w, h, m_gpuEnv.dev);
	wgpuSurfaceConfigure(m_gpuEnv.surf, &config);
	#endif
	
	// Create the Attachments and Descriptors
	this->CreateAttachments();
	this->CreateDescriptors();
	
	// Create the Bindings
	//this->CreateBindings();

	// Load a Shader
	m_shader.Load(m_gpuEnv.dev, "basic3d_uniform.wgsl");
	// Create the Render Pipeline
	this->CreatePipeline();
	
	// Create a Projection Matrix
	//this->CreateProjection();
	
	// Load a Mesh
	m_mesh.Load(m_gpuEnv.dev, m_gpuEnv.queue);
	puts("Initialized the App");
}

void App::Quit()
{
	if (m_projBuffer != nullptr)
	{
		// Release the Projection Buffer
		wgpuBufferRelease(m_projBuffer);
		m_projBuffer = nullptr;
	}
	
	// Release the Bindings
	if (m_bind.bindGroup != nullptr)
	{
		wgpuBindGroupRelease(m_bind.bindGroup);
		m_bind.bindGroup = nullptr;
	}
	if (m_bind.bindGroupLayout != nullptr)
	{
		wgpuBindGroupLayoutRelease(m_bind.bindGroupLayout);
		m_bind.bindGroupLayout = nullptr;
	}
	
	// Quit WebGPU
	quitGPUEnv(m_gpuEnv);
	// Destroy the Window
	destroyWindow(m_wnd);
	m_wnd = nullptr;
	puts("Quit the App");
}

bool App::IsRunning()
{
	#ifndef __EMSCRIPTEN__
	return (glfwWindowShouldClose(m_wnd) == false);
	#else
	return true;
	#endif
}

void App::SetDefaults()
{
	// Set Default Values
	m_bind.binding = {};
	m_bind.bindingLayout = {};
	m_bind.bindGroupLayout = nullptr;
	m_bind.bindGroup = nullptr;
	m_projBuffer = nullptr;
}

void App::SetBindGroup(const WGPURenderPassEncoder& renderPass)
{
	if (m_bind.bindGroup != nullptr)
	{
		wgpuRenderPassEncoderSetBindGroup(renderPass, 0, m_bind.bindGroup, 0, nullptr);
		puts("Set the Bind Group");
	}
}

void App::CreateAttachments()
{
	// ATTACHMENTS
	// Render Pass Color Attachment
	m_renderPassColorAttach = createRenderPassColorAttach();
	puts("Created the Attachments");
}

void App::CreateDescriptors()
{
	// DESCRIPTORS
	// Command Encoder Descriptor
	m_descriptors.encoderDesc = createEncoderDesc();
	// Render Pass Descriptor
	m_descriptors.renderPassDesc = createRenderPassDesc(m_renderPassColorAttach);
	// Command Buffer Descriptor
	m_descriptors.cmdBufferDesc = createCmdBufferDesc();
	puts("Created the Descriptors");
}

void App::CreateBindings()
{
	// Create the binding
	m_bind.binding = createBinding(sizeof(m_proj), m_projBuffer);
	// Create the Binding Layout
	m_bind.bindingLayout = createLayoutBinding();
	// Create the Bind Group Layout
	m_bind.bindGroupLayout = createLayoutBindGroup(m_gpuEnv.dev, &m_bind.bindingLayout);
	// Create the Bind Group
	m_bind.bindGroup = createBindGroup(m_gpuEnv.dev, m_bind.bindGroupLayout, &m_bind.binding);
}

void App::CreatePipeline()
{
	// Create the Render Pipeline
	WGPUShaderModule& shaderMod = m_shader.GetShaderMod();
	m_gpuEnv.pipeline = createRenderPipeline(m_gpuEnv.dev, shaderMod, nullptr);
}

void App::CreateProjection()
{
	// Create the Projection Matrix
	mat4x4_perspective(m_proj, 60.0f, 4 / 3.0f, 0.1f, 100.0f);
	m_projBuffer = createBufferMat4x4(m_gpuEnv.dev, m_gpuEnv.queue, m_proj);
}

void App::EventLoop()
{
	#ifndef __EMSCRIPTEN__
	// Check whether the user clicked on the close button (and any other
	// mouse/key event, which we don't use so far)
	glfwPollEvents();
	#endif
}

void App::Cls()
{
	// Get the next Target Texture View
	getNextTargetView(m_gpuEnv.surf, &m_gpuEnv.targetView);
	// Set the Target Texture View to Color Attachment
	m_renderPassColorAttach.view = m_gpuEnv.targetView;
}

void App::Flip()
{
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(m_gpuEnv.surf);
	#endif
}

void App::RenderPass(const WGPUCommandEncoder& encoder)
{
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass;
	renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &m_descriptors.renderPassDesc);
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_gpuEnv.pipeline);
	
	// {{Set the binding group here!}}
	//this->SetBindGroup(renderPass);
	
	// {{Draw}}
	m_mesh.Draw(renderPass);

	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void App::Draw()
{
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
}

void App::MainLoopGLFW()
{
	// Main Loop for GLFW
	while (this->IsRunning())
	{
		// Run the Event Loop
		this->EventLoop();
		
		// Clear
		this->Cls();
		// Draw
		this->Draw();
		// Flip
		this->Flip();
	}
}

void App::MainLoopEM()
{
	// Equivalent of the Main Loop when using Emscripten:
	auto callback = [](void *arg)
	{
		App* pApp = reinterpret_cast<App*>(arg);
		// Run the Event Loop
		pApp->EventLoop();
		
		// Clear
		pApp->Cls();
		// Draw
		pApp->Draw();
		// Flip
		pApp->Flip();
	};
	
	// Set the Main Loop
	//emscripten_set_main_loop_arg(callback, this, 0, true);
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
