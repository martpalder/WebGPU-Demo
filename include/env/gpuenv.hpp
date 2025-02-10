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
} GPUEnv;

GPUEnv initGPUEnv(GLFWwindow* wnd);
void quitGPUEnv(GPUEnv gpuEnv);

#endif	// GPUENV_HPP_INCLUDED
