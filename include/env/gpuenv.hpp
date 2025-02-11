#pragma once
#ifndef GPUENV_HPP_INCLUDED
#define GPUENV_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>

typedef struct {
	WGPUInstance inst;
	WGPUSurface surf;
	WGPUDevice dev;
	WGPUQueue queue;
	WGPURenderPipeline pipeline;
} GPUEnv;

GPUEnv initGPUEnv(GLFWwindow* wnd);
void quitGPUEnv(const GPUEnv& gpuEnv);

#endif	// GPUENV_HPP_INCLUDED
