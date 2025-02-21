#include "./world.hpp"
#include "./texture.hpp"
#include "./view.hpp"
#include "./pipeline.hpp"
#include "./stdafx.h"

World::World()
{
	// Setup Projection Matrix
	mat4x4_identity(m_p);
	mat4x4_identity(m_vp);
	mat4x4_perspective(m_p, 90.0f, 4 / 3.0f, 0.1f, 100.0f);
	
	// Update the Camera
	m_cam.Update();
	
	// Combine the View-Projection Matrix
	mat4x4_mul(m_vp, m_p, m_cam.GetView());	// Check correct ordering
	puts("Created the World");
}

World::~World()
{
	// Clear the Actors
	m_actors.clear();
	
	// Release the Render Pipeline
	wgpuRenderPipelineRelease(m_pipeline);
	printRelease("Render Pipeline");
	
	// Release the Target View
	wgpuTextureViewRelease(m_targetView);
	printRelease("Target View");
	
	// Release the Layouts
	if (m_layouts.bindGroupLayout != nullptr)
	{
		wgpuBindGroupLayoutRelease(m_layouts.bindGroupLayout);
		m_layouts.bindGroupLayout = nullptr;
		printRelease("Bind Group Layout");
	}
	
	// Release the Depth View
	wgpuTextureViewRelease(m_depthView);
	printRelease("Depth View");
	// Release the Depth Texture
	wgpuTextureRelease(m_depthTexture);
	printRelease("Depth Texture");
	puts("Destroyed the World");
}

void World::Init(const WGPUDevice& device,
const WGPUShaderModule& shaderMod,
int w, int h)
{
	this->CreateLayouts(device);
	this->CreatePipeline(device, shaderMod, w, h);
}

const Camera* World::GetCam() const
{
	return &m_cam;
}

void World::CreateLayouts(const WGPUDevice& device)
{
	m_layouts.bindingLayout = createLayoutBinding(sizeof(mat4x4));
	m_layouts.bindGroupLayout = createLayoutBindGroup(device, &m_layouts.bindingLayout);
	m_layouts.pipelineLayout = createLayoutPipeline(device, &m_layouts.bindGroupLayout);
}

void World::CreatePipeline(const WGPUDevice& device,
const WGPUShaderModule& shaderMod,
int w, int h)
{
	// Create a Depth Texture and View
	WGPUTextureFormat depthFormat = WGPUTextureFormat_Depth16Unorm;
	m_depthTexture = createDepthTexture(device, w, h, depthFormat);
	m_depthView = createDepthView(m_depthTexture);
	
	// Create the Attachments
	m_attach.colorAttach = createColorAttach(0.1f, 0.1f, 0.1f);
	m_attach.depthStencilAttach = createDepthStencilAttach(m_depthView);
	puts("Created the Attachments");
	
	// Create the Descriptors
	bool bDepthStencil = true;
	m_desc = createDescriptors(m_attach, bDepthStencil);
	
	// Create the Pipeline States
    States states = createStates(shaderMod, depthFormat);
    // Create the Pipeline Descriptor
	WGPURenderPipelineDescriptor pipelineDesc = createRenderPipelineDesc(states, bDepthStencil);
    
    // PIPELINE LAYOUT
	// Assign the PipelineLayout to the RenderPipelineDescriptor's layout field
	pipelineDesc.layout = createLayoutPipeline(device, &m_layouts.bindGroupLayout);
	puts("Assigned the PipelineLayout to the RenderPipelineDescriptor");
	
	// Create the Render Pipeline
	m_pipeline = createRenderPipeline(device, pipelineDesc);
}

Actor* World::AddActor(const GPUEnv& gpuEnv, float x, float y, float z,
const char* tag)
{
	Actor* pActor = new Actor(x, y, z);
	pActor->Init(gpuEnv, m_vp);
	pActor->SetTag(tag);
	m_actors.push_back(pActor);
	printf("Added an Actor: '%s'\n", tag);
	
	return pActor;
}

void World::Cls(const WGPUSurface& surf)
{
	// Get the next Target Texture View
	getNextTargetView(surf, &m_targetView);
	// Assign the Target Texture View to Color Attachment
	m_attach.colorAttach.view = m_targetView;
}

void World::Flip(const WGPUSurface& surf)
{
	#ifndef __EMSCRIPTEN__
	wgpuSurfacePresent(surf);
	#endif
}

void World::RenderPass(const WGPUCommandEncoder& encoder,
const Descriptors& desc)
{
	// {{Begin the Render Pass}}
	WGPURenderPassEncoder renderPass;
	renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &desc.renderPassDesc);
	
	// {{Set the Render Pipeline}}
	wgpuRenderPassEncoderSetPipeline(renderPass, m_pipeline);
	
	// {{Draw}}
	for (Actor* pActor : m_actors)
	{
		pActor->Draw(renderPass);
	}
	
	// {{End and release the Render Pass}}
	wgpuRenderPassEncoderEnd(renderPass);
	wgpuRenderPassEncoderRelease(renderPass);
}

void World::SubmitCommand(const WGPUQueue& queue,
const WGPUCommandBuffer& command)
{
	puts("Submitting command...");
	wgpuQueueSubmit(queue, 1, &command);
	wgpuCommandBufferRelease(command);
	puts("Command submitted.");
}

void World::Update(const WGPUQueue& queue)
{
	// Update the Camera
	m_cam.Update();
	// Combine the View-Projection Matrix
	mat4x4_mul(m_vp, m_p, m_cam.GetView());	// Check correct ordering
	
	// Update all Actors
	for (Actor* pActor : m_actors)
	{
		pActor->Update(queue, m_vp);
	}
}

void World::Draw(const WGPUDevice& device,
const WGPUQueue& queue)
{
	// {{Create a Command Encoder}}
	WGPUCommandEncoder encoder;
	encoder = wgpuDeviceCreateCommandEncoder(device, &m_desc.encoderDesc);
	
	// {{Do a Render Pass}}
	this->RenderPass(encoder, m_desc);
	
	// {{Finish encoding the Command}}
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &m_desc.cmdBufferDesc);
	// {{Release the Command Encoder}}
	wgpuCommandEncoderRelease(encoder);
	
	// {{Submit the Command}}
	this->SubmitCommand(queue, command);
}
