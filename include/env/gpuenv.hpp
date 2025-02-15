#pragma once
#ifndef GPUENV_HPP_INCLUDED
#define GPUENV_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <cstdio>

typedef struct {
	WGPUInstance inst;
	WGPUSurface surf;
	WGPUDevice dev;
	WGPUQueue queue;
	WGPURenderPipeline pipeline;
	WGPUTextureView targetView;
} GPUEnv;

WGPURequestAdapterOptions createAdapterOpts(const WGPUSurface& surf);

GPUEnv initGPUEnv(GLFWwindow* wnd);
void quitGPUEnv(const GPUEnv& gpuEnv);

inline void printRelease(const char* name)
{
	printf("Released the %s\n", name);
}

#endif	// GPUENV_HPP_INCLUDED
